// --------------------------------------------------------------------------------------------------------------------
// Module for handling network messages to load levels.
//
// @module level_manager.js
// --------------------------------------------------------------------------------------------------------------------

.import "../js/logging.js" as Logging
.import Precursors 1.0 as Precursors

var logger = new Logging.Logger("level_manager");

var lastRequestedSceneID;

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
	horde3d.sceneLoaded.connect(this.onSceneLoaded);
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
			lastRequestedSceneID = level;

			if(horde3d.isSceneLoaded(lastRequestedSceneID))
			{
				logger.debug("Scene \'%1\' already loaded; switching scenes.", lastRequestedSceneID);
				horde3d.switchScene(lastRequestedSceneID, true);
			}
			else
			{
				logger.debug("Loading scene \'%1\'...", lastRequestedSceneID);
				horde3d.loadScene(lastRequestedSceneID);
			} // end if
		} // end if
	} // end if
} // end handleIncomingEvent

function onSceneLoaded(sceneID, scene)
{
	if(sceneID === lastRequestedSceneID)
	{
		logger.debug("Loaded scene \'%1\'; switching scenes.", sceneID);
		horde3d.switchScene(lastRequestedSceneID);
	}
	else
	{
		logger.debug("Loaded scene \'%1\', which is not the desired scene; ignoring.", sceneID);
	} // end if
} // end onSceneLoaded

// --------------------------------------------------------------------------------------------------------------------
