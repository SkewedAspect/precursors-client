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

function connectSignals()
{
    controls.context('flightsim').digitalSlot('quit').stateChanged.connect(function()
    {
        logger.info("Exiting...");
        gameWindow.close();
    });

    controls.context('flightsim').digitalSlot('grabMouse').stateChanged.connect(function()
    {
        gameWindow.grabMouse = controls.context('flightsim').digitalSlot('grabMouse').state;
    });

    controls.context('flightsim').digitalSlot('fire').stateChanged.connect(function()
    {
        console.log("Got 'fire'.", controls.context('flightsim').digitalSlot('fire').state);
    });

    controls.context('flightsim').digitalSlot('coast').stateChanged.connect(function()
    {
        console.log("Got 'coast'.", controls.context('flightsim').digitalSlot('coast').state);
    });

    controls.context('flightsim').digitalSlot('headlights').stateChanged.connect(function()
    {
        console.log("Got 'headlights'.", controls.context('flightsim').digitalSlot('headlights').state);
    });

    controls.context('flightsim').analogSlot('sideslip').valueChanged.connect(_.partial(sendCommand, 'sideslip'));
    controls.context('flightsim').analogSlot('lift').valueChanged.connect(_.partial(sendCommand, 'lift'));
    controls.context('flightsim').analogSlot('throttle').valueChanged.connect(_.partial(sendCommand, 'throttle'));

    controls.context('flightsim').analogSlot('heading').valueChanged.connect(_.partial(sendCommand, 'yaw'));
    controls.context('flightsim').analogSlot('pitch').valueChanged.connect(_.partial(sendCommand, 'pitch'));
    controls.context('flightsim').analogSlot('roll').valueChanged.connect(_.partial(sendCommand, 'roll'));
} // end connectSignals
