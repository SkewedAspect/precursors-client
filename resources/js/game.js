// ---------------------------------------------------------------------------------------------------------------------
// A collection of modules and logic that we consider "the game".
//
// @module game.js
// ---------------------------------------------------------------------------------------------------------------------

.import "../js/logging.js" as Logging
.import "../js/entity_manager.js" as EntityManager
.import "../js/level_manager.js" as LevelManager

var logger = new Logging.Logger("game");

// ---------------------------------------------------------------------------------------------------------------------

function Game()
{
	logger.info("Starting the game...");

	this.entityMan = new EntityManager.EntityManager();
	this.levelMan = new LevelManager.LevelManager();
} // end Game

// ---------------------------------------------------------------------------------------------------------------------
