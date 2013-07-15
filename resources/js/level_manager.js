// --------------------------------------------------------------------------------------------------------------------
// Module for handling network messages to load levels.
//
// @module level_manager.js
// --------------------------------------------------------------------------------------------------------------------

.import "../js/logging.js" as Logging
.import Precursors 1.0 as Precursors

var logger = new Logging.Logger("level_manager");

// --------------------------------------------------------------------------------------------------------------------
// Initialization
// --------------------------------------------------------------------------------------------------------------------

connectSignals();
logger.debug("LevelManager initialized.")

// --------------------------------------------------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------------------------------------------------

function connectSignals()
{
	networking.incomingMessage.connect(this.handleIncomingEvent);
} // end connectSignals

function handleIncomingEvent(channel, event)
{
	if(channel == "level")
	{
		var level = event.level;

		if(event.type == "setZone")
		{
			logger.debug("Incoming setZone: %1", JSON.stringify(level));

			//XXX: HACK! This is here because the server sends us a hard-coded value for the level. This is the correct
			// level for the python client, but not for us.
			if(level == "zones/test/TestArea.json")
			{
				logger.warning("Got old test level; loading current test zone instead.");
				level = "scenes/asteroids/asteroids.scene.xml";
			} // end if

			var oldScene = horde3d.scene;
			horde3d.scene = horde3d.root.loadScene(level);
			logger.debug("Loaded \'%1\'.", level);

			if(oldScene)
			{
				oldScene.remove();
			} // end if
		} // end if
	} // end if
} // end handleIncomingEvent

// --------------------------------------------------------------------------------------------------------------------
