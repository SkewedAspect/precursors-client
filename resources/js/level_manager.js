// ---------------------------------------------------------------------------------------------------------------------
// Module for handling network messages to load levels.
//
// @module level_manager.js
// ---------------------------------------------------------------------------------------------------------------------

.import "../js/logging.js" as Logging

var logger = new Logging.Logger("level_manager");

function LevelManager()
{
    this.connectSignals();
    logger.debug("LevelManager initialized.")
} // end LevelManager

LevelManager.prototype.connectSignals = function()
{
    networking.incommingMessage.connect(this.handleIncomingEvent);
} // end connectSignals

LevelManager.prototype.handleIncomingEvent = function(channel, event)
{
	if(channel == "level")
	{
		if(event.type == "setZone")
		{
			logger.debug("Incoming setZone: %1", JSON.stringify(event.level));

			//XXX: HACK! This is here because the server sends us a hard-coded value for the level. This is the correct level
			// for the python client, but not for us.
			if(event.level == "zones/test/TestArea.json")
			{
				logger.warning("Got old test level; loading current test zone instead.");
				event.level = "resources/scenes/asteroids/asteroids.scene.xml";
			} // end if

			// TODO: load the level?
			logger.debug("Would load level \'%1\' here!", event.level);
		} // end if
	} // end if
} // end handleIncomingEvent

// ---------------------------------------------------------------------------------------------------------------------

