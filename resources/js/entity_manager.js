// ---------------------------------------------------------------------------------------------------------------------
// Module for handling network messages and update entities.
//
// @module entity_manager.js
// ---------------------------------------------------------------------------------------------------------------------

.import Precursors.Networking 1.0 as Networking
.import "../js/logging.js" as Logging

var logger = new Logging.Logger("entity_manager");

// ---------------------------------------------------------------------------------------------------------------------
// Initialization
// ---------------------------------------------------------------------------------------------------------------------

var localEntities = {};
var ignoredEntities = [];
var inhabitedID;

connectSignals();
logger.debug("EntityManager initialized.")

// ---------------------------------------------------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------------------------------------------------

function connectSignals()
{
    networking.incommingMessage.connect(handleIncomingEvent);
} // end connectSignals

function updateEntity(event, entity)
{
	var entityID = event.id;
	var entity = entity || localEntities[entityID];

	//logger.debug("Update: %1", JSON.stringify(event));

	//TODO: Set the behavior.
	logger.debug("Should load behavior: \'%1\'", event.behavior);

	//-----------------------------------------------------------------------------------------------------------------
	//TODO: This code needs to go someplace else, and be turned into something a bit more general.
	//-----------------------------------------------------------------------------------------------------------------

	if(event.state.modelDef)
	{
		var modelPath = event.state.modelDef.model;

		//XXX: HACK! This is here because the server sends us a hard-coded value for the model. This is the correct model
		// for the python client, but not for us.
		if(modelPath == "Ships/ares")
		{
			logger.warning("Got old model; loading current model instead.");
			modelPath = "models/ares/ares.scene.xml";
		} // end if

		// Load model
		entity.loadModel(modelPath);

		logger.debug("Loaded model: \'%1\'.", modelPath);
	} // end state.modelDef

	//TODO: We should be handling this in the entity code.
	if(event.state.position)
	{
		var position = event.state.position;
		entity.setPos(position[0], position[1], position[2]);
	} // end if

	//TODO: We should be handling this in the entity code.
	if(event.state.orientation)
	{
		var orientation = event.state.orientation;

		entity.heading = orientation[0] * (180/Math.PI);
		entity.pitch = orientation[1] * (180/Math.PI);
		entity.roll = orientation[2] * (180/Math.PI);
	} // end if

	//-----------------------------------------------------------------------------------------------------------------

	// Update the state with
	entity.updateState(event.state);
} // end updateEntity

function loadEntity(event)
{
	var entity = horde3d.root.newGroup(event.id)
	localEntities[event.id] = entity;

	// Now we call update Enity to actually handle updating things.
	updateEntity(event, entity);
} // end updateEntity

// ---------------------------------------------------------------------------------------------------------------------
// Event handling
// ---------------------------------------------------------------------------------------------------------------------

function handleIncomingEvent(channel, event)
{
	if(channel == "entity")
	{
		// In order to avoid confusion; event.id is the id of the entity this event is for, *not* the id of the event.
		var entityID = event.id;

		switch(event.type)
		{
			case "update":
			{
				logger.notify("Incoming Update for Entity \'%1\'", entityID);

				if(ignoredEntities.contains(entityID))
				{
					// This is an ignored entity; we don't touch it.
					break;
				} // end if

				if(localEntities[entityID] == undefined)
				{
					logger.debug("Entity not in list of local entities. Requesting full update.");

					// Ignore future updated for this entity until we get the full update.
					ignoredEntities.push(entityID);

					// Send a request for the full update for this entity
					var fullUpReq = networking.buildRequest("entity", { type: "full", id: entityID }, Networking.PChannels.CM_RELIABLE);
					fullUpReq.reply.connect(handleFullUpdateReply);
					fullUpReq.send();

					function handleFullUpdateReply(confirmed)
					{
						var reply = fullUpReq.replyMessage;

						if(confirmed)
						{
							logger.debug("Full update for entity \'%1\' confirmed!", entityID);

							// Massage the message into looking like an event.
							reply.type = "full";

							// Process it like a normal event
							handleIncomingEvent(channel, reply);

							// Remove it from the ignored entities list
							ignoredEntities.removeAll(entityID);
						}
						else
						{
							logger.warning("Full update for entity \'%1\' denied! Reason: %2", entityID, reply.reason);
						} // end if
					} // end handleFullUpdateReply

				}
				else
				{
					updateEntity(event);
				} // end if

				break;
			} // end update

			case "inhabit":
			case "full":
			{
				logger.notify("Incoming full/inhabit update for entity \'%1\'", entityID);

				if(localEntities.contains(entityID))
				{
					logger.debug("Updating known entity id \'%1\' with a full update.", entityID);

					// We already have this entity, so we just update it.
					updateEntity(event);
				}
				else
				{
					logger.debug("Got full/inhabit update for new entity id \'%1\'", entityID);

					// We're getting a full update for an entity we don't have, currently.
					loadEntity(event)
				} // end if

				if(event.type == "inhabit")
				{
					inhabitedID = entityID;

					var entity = localEntities[entityID];

					// Set our avatar.
					horde3d.avatar = entity;

					// Reparent the camera dolly to the avatar.
					mainWindow.camDolly.parent = entity;
				} // end if

				break;
			} // end full/inhabit

			case "remove":
			{
				logger.debug("Removing entity \'%1\'.", entityID);

				// Remove the entity from our local ones.
				delete localEntities[entityID];

				break;
			} // end remove

			case "create":
			{
				logger.debug("Creating entity \'%1\'.");

				loadEntity(event);

				break;
			} // end create

			default: {
				logger.warning("Unhandled Entity Event: %1", JSON.stringify(event));
			}
		} // end switch
	} // end if
} // end handleIncomingEvent

// ---------------------------------------------------------------------------------------------------------------------
