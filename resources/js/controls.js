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

function connectSignals()
{
    defineContextSlots('flightsim', {
        digital: {
            quit: function()
            {
                logger.info("Exiting...");
                gameWindow.close();
            },

            grabMouse: function(state)
            {
                gameWindow.grabMouse = state;
            },

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
            roll: _.partial(sendCommand, 'roll')
        }
    });
} // end connectSignals
