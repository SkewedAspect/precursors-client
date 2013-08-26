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

function defineContextSlots(contextName, slots)
{
    var context = controls.context(contextName);

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

function cameraPitch(value)
{
    mainWindow.camDolly.rotatePitch(value);
} // end cameraPitch

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
            mainWindow.camDolly.orientation = Qt.quaternion(1, 0, 0, 0); // Reset orientation to the default.
            mainWindow.camDolly.rotatePitch(-11); // Rotate to match default camera orientation for flightsim.
            mainWindow.camDolly.parent = horde3d.avatar;
        } // end if
    }); // end isActiveChanged

    defineContextSlots('flightsim', {
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

            heading: _.partial(sendCommand, 'yaw'),
            pitch: _.partial(sendCommand, 'pitch'),
            roll: _.partial(sendCommand, 'roll'),

            cameraHeading: function(value)
            {
                mainWindow.camDolly.rotateHeading(value);
            },
            cameraPitch: cameraPitch,
            cameraZoom: cameraZoom
        }
    });
} // end defineFlightsim

function defineCapital()
{
    var context = controls.context('capital');
    var slots = {
        reorient: context.digitalSlot('reorient')
    };
	var responsiveness = 0.1;
	var camTurntable;
    var shipRotateTimer;

    function updateShipRotation()
    {
        var avatar = horde3d.avatar;
        var avatarQuat = avatar.orientation;

        //var totalCamOrientation = math3d.quatMult(mainWindow.camDolly.orientation, camTurntable.orientation);
        var totalCamOrientation = camTurntable.orientation;
        var lerped = math3d.slerp(totalCamOrientation, avatarQuat, responsiveness);

        /*
        var lerpDerivative = math3d.quatNormalized(
                math3d.quatMult(
                    math3d.quatMult(
                        math3d.quatLog(
                            math3d.quatMult(
                                avatarQuat,
                                math3d.quatReciprocal(totalCamOrientation)
                                )
                            ),
                            lerped
                        ),
                    math3d.quatReciprocal(avatarQuat)
                    )
                );
        */

        /*
        var lerpDerivative = math3d.quatNormalized(
                math3d.quatMult(
                    math3d.quatMult(
                        math3d.quatLog(
                            math3d.quatMult(
                                totalCamOrientation,
                                math3d.quatReciprocal(avatarQuat)
                                )
                            ),
                            lerped
                        ),
                    math3d.quatReciprocal(avatarQuat)
                    )
                );
        */

        /*
        var lerpDerivative = math3d.quatNormalized(
                math3d.quatLog(
                    math3d.quatMult(
                        totalCamOrientation,
                        math3d.quatReciprocal(avatarQuat)
                        )
                    )
                );
        */

        //var desiredRotation = lerpDerivative;

        /*
        console.log(math3d.quatSubtract(totalCamOrientation, avatarQuat));

        var desiredRotation = math3d.quatNormalized(
                math3d.quatMult(
                    math3d.quatMult(
                        math3d.quatSubtract(avatarQuat, totalCamOrientation),
                        responsiveness
                        ),
                    math3d.quatReciprocal(avatarQuat)
                    )
                );
        */

        var desiredRotation = math3d.quatNormalized(
                math3d.quatMult(
                    math3d.quatMult(
                        math3d.quatMult(
                            totalCamOrientation,
                            math3d.quatConjugate(avatarQuat)
                            ),
                        math3d.quatConjugate(avatarQuat)
                        ),
                    responsiveness
                    )
                );

        console.log('yaw', math3d.quatToHeading(desiredRotation));
        console.log('pitch', math3d.quatToPitch(desiredRotation));
        console.log('roll', math3d.quatToRoll(desiredRotation));

        sendCommand('yaw', math3d.quatToHeading(desiredRotation));
        sendCommand('pitch', math3d.quatToPitch(desiredRotation));
        sendCommand('roll', math3d.quatToRoll(desiredRotation));
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

            // Reset orientation to the default, then only apply the heading from the ship.
            camTurntable.orientation = Qt.quaternion(1, 0, 0, 0);
            camTurntable.rotateHeading(horde3d.avatar.heading * 180 / Math.PI);

            mainWindow.camDolly.parent = camTurntable;
        } // end if
    }); // end isActiveChanged

    defineContextSlots('capital', {
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

                if(state)
                {
                    shipRotateTimer.start();
                }
                else
                {
                    shipRotateTimer.stop();
                    sendCommand('yaw', 0);
                    sendCommand('pitch', 0);
                    sendCommand('roll', 0);
                } // end if
            },

            headlights: function(state)
            {
                console.log("Got 'headlights'.", state);
            }
        },
        analog: {
            throttle: _.partial(sendCommand, 'throttle'),
            heading: _.partial(sendCommand, 'yaw'),
            pitch: _.partial(sendCommand, 'pitch'),
            roll: _.partial(sendCommand, 'roll'),

            cameraHeading: function(value)
            {
                camTurntable.rotateHeading(value);
            },
            cameraPitch: cameraPitch,
            cameraZoom: cameraZoom
        }
    });
} // end defineCapital
