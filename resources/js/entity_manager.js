// ---------------------------------------------------------------------------------------------------------------------
// Module for handling network messages and update entities.
//
// @module entity_manager.js
// ---------------------------------------------------------------------------------------------------------------------

.import "../js/logging.js" as Logging

var logger = new Logging.Logger("entity_manager");

function EntityManager()
{
    this.connectSignals();
    logger.debug("EntityManager initialized.")
} // end EntityManager

EntityManager.prototype.connectSignals = function()
{
    networking.incommingMessage.connect(this.handleIncomingEvent);
} // end connectSignals

EntityManager.prototype.handleIncomingEvent = function(event)
{
    logger.debug("Incoming Event: %1", JSON.stringify(event));
} // end handleIncomingEvent

// ---------------------------------------------------------------------------------------------------------------------
