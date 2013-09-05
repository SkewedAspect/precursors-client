// --------------------------------------------------------------------------------------------------------------------
// Module for passing user control events to the server.
//
// @module controls.js
// --------------------------------------------------------------------------------------------------------------------

.import Precursors.Networking 1.0 as Networking
.import "../js/logging.js" as Logging

Qt.include("./vendor/lodash.js");

var logger = new Logging.Logger("controls");

// --------------------------------------------------------------------------------------------------------------------
// Initialization
// --------------------------------------------------------------------------------------------------------------------

connectSignals();
logger.debug("Initialized.")

// --------------------------------------------------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------------------------------------------------

function sendCommand(commandName /*, ... */)
{
    networking.sendEvent('input', {
        type: 'command',
        name: commandName,
        args: _.rest(arguments)
    }, Networking.PChannels.CM_RELIABLE);
} // end sendCommand

function defineContextSlots(context, slots)
{
    _.forIn(slots.digital, function(handler, slotName)
    {
        var slot = context.digitalSlot(slotName);
        slot.stateChanged.connect(wrapDigital(slot, handler));
    });

    function wrapDigital(slot, handler)
    {
        return function()
        {
            handler(slot.state);
        } // end function
    } // end wrapDigital

    _.forIn(slots.analog, function(handler, slotName)
    {
        var slot = context.analogSlot(slotName);
        slot.valueChanged.connect(wrapAnalog(slot, handler));
    });

    function wrapAnalog(slot, handler)
    {
        return function()
        {
            handler(slot.value);
        } // end function
    } // end wrapAnalog
} // end defineContextSlots

function momentary(handler)
{
    return function(state)
    {
        if(state)
        {
            handler(state);
        } // end if
    }; // end function
} // end momentary

function connectSignals()
{
    defineFlightsim();
    defineCapital();
} // end connectSignals

function cameraZoom(value)
{
    mainWindow.camera.setPos(0, 0, value);
} // end cameraZoom

function defineFlightsim()
{
    var context = controls.context('flightsim');

    context.isActiveChanged.connect(function()
    {
        if(context.isActive)
        {
            // This context was just selected.
            mainWindow.camDolly.orientation = identityQuat(); // Reset orientation to the default.
            mainWindow.camDolly.rotatePitch(-11); // Rotate to match default camera orientation for flightsim.
            mainWindow.camDolly.parent = horde3d.avatar;
        } // end if
    }); // end isActiveChanged

    defineContextSlots(context, {
        digital: {
            quit: momentary(function()
            {
                logger.info("Exiting...");
                gameWindow.close();
            }),

            grabMouse: function(state)
            {
                gameWindow.grabMouse = state;
            },

            switchToCapitalMode: momentary(function(state)
            {
                controls.setCurrentContext('capital');
            }),

            fire: function(state)
            {
                console.log("Got 'fire'.", state);
            },

            coast: function(state)
            {
                console.log("Got 'coast'.", state);
            },

            headlights: function(state)
            {
                console.log("Got 'headlights'.", state);
            }
        },
        analog: {
            sideslip: _.partial(sendCommand, 'sideslip'),
            lift: _.partial(sendCommand, 'lift'),
            throttle: _.partial(sendCommand, 'throttle'),

            heading: _.partial(sendCommand, 'heading'),
            pitch: _.partial(sendCommand, 'pitch'),
            roll: _.partial(sendCommand, 'roll'),

            cameraHeading: function(value)
            {
                mainWindow.camDolly.rotateHeading(value * Math.PI);
            },
            cameraPitch: function(value)
            {
                mainWindow.camDolly.rotatePitch(value * Math.PI);
            },
            cameraZoom: cameraZoom
        }
    });
} // end defineFlightsim

function identityQuat()
{
    return Qt.quaternion(1, 0, 0, 0);
} // end identityQuat

function eulerToQuat(heading, pitch, roll)
{
    var c1 = Math.cos(heading / 2);
    var c2 = Math.cos(roll / 2);
    var c3 = Math.cos(pitch / 2);
    var s1 = Math.sin(heading / 2);
    var s2 = Math.sin(roll / 2);
    var s3 = Math.sin(pitch / 2);

    var w = c1 * c2 * c3 - s1 * s2 * s3;
    var x = s1 * s2 * c3 + c1 * c2 * s3;
    var y = s1 * c2 * c3 + c1 * s2 * s3;
    var z = c1 * s2 * c3 - s1 * c2 * s3;

    return Qt.quaternion(w, x, y, z);
} // end eulerToQuat

function rotationFromTo(from, to)
{
    return math3d.quatNormalized(
            math3d.quatMult(
                math3d.quatReciprocal(from),
                to
                )
            );
} // end rotationFromTo

function scaleQuat(quat, scale)
{
    var scalar = scale * math3d.quatScalar(quat);
    return math3d.quatNormalized(math3d.quatFromScalarAndVector(scalar, math3d.quatVector(quat)));
} // end scaleQuat

function setPosNegBars(posBar, negBar, value, limit)
{
    if(value > 0)
    {
        posBar.item.progress = Math.min(limit, value);
        negBar.item.progress = 0;
    }
    else
    {
        posBar.item.progress = 0;
        negBar.item.progress = Math.min(limit, -value);
    } // end if
} // end setPosNegBars

function scaleQuat(quat, factor)
{
    return math3d.slerp(identityQuat(), quat, factor);
} // end scaleQuat

function quatAngle(quat)
{
    //return 2 * Math.acos(math3d.quatScalar(quat));
    return 2 * Math.atan2(math3d.vectorLength(math3d.quatVector(quat)), math3d.quatScalar(quat));
} // end quatAngle

function defineCapital()
{
    var context = controls.context('capital');
    var slots = {
        reorient: context.digitalSlot('reorient')
    };
	var responsiveness = 1;
	var camTurntable;
    var shipRotateTimer;
    var targetHeadingQuat = identityQuat();
    var targetPitchQuat = identityQuat();
    var headingVelQuat = identityQuat();
    var pitchVelQuat = identityQuat();

    function updateShipRotation()
    {
        var avatar = horde3d.avatar;
        var avatarQuat = avatar.orientation;

        if(slots.reorient.state)
        {
            // Set the target orientation to match the camera.
            targetHeadingQuat = camTurntable.orientation;
            targetPitchQuat = mainWindow.camDolly.orientation;
        }
        else
        {
            // Update the target orientation based on the current heading/pitch velocities from input.
            //FIXME: Use the frame time here instead of 0.1!
            targetHeadingQuat = math3d.quatMult(targetHeadingQuat, scaleQuat(headingVelQuat, 0.1));
            targetPitchQuat = constrainPitch(math3d.quatMult(targetPitchQuat, scaleQuat(pitchVelQuat, 0.1)));
        } // end if

        var targetOrientation = math3d.quatMult(targetHeadingQuat, targetPitchQuat);
        var desiredRotation = rotationFromTo(
                avatarQuat,
                targetOrientation
                );

        var rotAngle = 2 * quatAngle(desiredRotation) / Math.PI;
        desiredRotation = scaleQuat(desiredRotation, Math.sqrt(Math.abs(rotAngle)) * responsiveness);

        var hprIdx = {
            heading: 0,
            pitch: 2,
            roll: 1
        };

        var tgtVelHPR = math3d.quatToHPR(desiredRotation);
        var tgtVelLimit = debugWindow.angularTgtVelMax;
        setPosNegBars(debugWindow.posTgtVelHeading, debugWindow.negTgtVelHeading, tgtVelHPR[hprIdx.heading], tgtVelLimit);
        setPosNegBars(debugWindow.posTgtVelPitch, debugWindow.negTgtVelPitch, tgtVelHPR[hprIdx.pitch], tgtVelLimit);
        setPosNegBars(debugWindow.posTgtVelRoll, debugWindow.negTgtVelRoll, tgtVelHPR[hprIdx.roll], tgtVelLimit);

        var camHPR = math3d.quatToHPR(targetOrientation);
        setPosNegBars(debugWindow.posCamHeading, debugWindow.negCamHeading, camHPR[hprIdx.heading], Math.PI);
        setPosNegBars(debugWindow.posCamPitch, debugWindow.negCamPitch, camHPR[hprIdx.pitch], Math.PI);
        setPosNegBars(debugWindow.posCamRoll, debugWindow.negCamRoll, camHPR[hprIdx.roll], Math.PI);

        var shipHPR = math3d.quatToHPR(avatarQuat);
        setPosNegBars(debugWindow.posShipHeading, debugWindow.negShipHeading, shipHPR[hprIdx.heading], Math.PI);
        setPosNegBars(debugWindow.posShipPitch, debugWindow.negShipPitch, shipHPR[hprIdx.pitch], Math.PI);
        setPosNegBars(debugWindow.posShipRoll, debugWindow.negShipRoll, shipHPR[hprIdx.roll], Math.PI);

        sendCommand('heading', tgtVelHPR[hprIdx.heading]);
        sendCommand('pitch', tgtVelHPR[hprIdx.pitch]);
        sendCommand('roll', tgtVelHPR[hprIdx.roll]);
    } // end updateShipRotation

    context.isActiveChanged.connect(function()
    {
        if(context.isActive)
        {
            // This context was just selected.

            if(!camTurntable)
            {
                camTurntable = horde3d.root.newGroup('camera turntable');

                // Bind 'camTurntable.pos' to 'avatar.pos'
                camTurntable.pos = Qt.binding(function() { return horde3d.avatar.pos });
            } // end if

            if(!shipRotateTimer)
            {
                shipRotateTimer = Qt.createQmlObject(
                        'import QtQuick 2.0; Timer {interval: 50; repeat: true; triggeredOnStart: true}',
                        mainWindow,
                        "shipRotateTimerDef"
                        );
                shipRotateTimer.triggered.connect(updateShipRotation);
            } // end if
            shipRotateTimer.start();

            // Reset orientation to the default, then only apply the heading from the ship.
            camTurntable.orientation = identityQuat();
            camTurntable.rotateHeading(horde3d.avatar.heading * 180 / Math.PI);

            mainWindow.camDolly.parent = camTurntable;

            // Set maximum values for rotation debugging controls.
            debugWindow.angularTgtVelMax = Math.PI * responsiveness;
        }
        else
        {
            shipRotateTimer.stop();
        } // end if
    }); // end isActiveChanged

    defineContextSlots(context, {
        digital: {
            quit: momentary(function()
            {
                logger.info("Exiting...");
                gameWindow.close();
            }),

            grabMouse: function(state)
            {
                gameWindow.grabMouse = state;
            },

            switchToFlightsimMode: momentary(function(state)
            {
                controls.setCurrentContext('flightsim');
            }),

            fire: function(state)
            {
                console.log("Got 'fire'.", state);
            },

            reorient: function(state)
            {
                console.log("Got 'reorient'.", state);
            },

            headlights: function(state)
            {
                console.log("Got 'headlights'.", state);
            }
        },
        analog: {
            throttle: _.partial(sendCommand, 'throttle'),

            heading: function(value)
            {
                headingVelQuat = eulerToQuat(value, 0, 0);
            },
            pitch: function(value)
            {
                pitchVelQuat = eulerToQuat(0, value, 0);
            },

            cameraHeading: function(value)
            {
                camTurntable.rotateHeading(value * Math.PI);
            },
            cameraPitch: function(value)
            {
                mainWindow.camDolly.rotatePitch(value * Math.PI);
                mainWindow.camDolly.orientation = constrainPitch(mainWindow.camDolly.orientation);
            },
            cameraZoom: cameraZoom
        }
    });

    function constrainPitch(quat)
    {
        if(quatAngle(quat) > Math.PI / 2)
        {
            return eulerToQuat(0, Math.PI / (math3d.quatX(quat) > 0 ? 2 : -2), 0);
        }
        else
        {
            return quat;
        } // end if
    } // end constrainPitch
} // end defineCapital
