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

function updateEntity(event)
{
	//TODO: this should actually update the entity.
	logger.warning("updateEntity not implemented!");
} // end updateEntity

function loadEntity(event)
{
	//TODO: this should actually update the entity.
	logger.warning("updateEntity not implemented!");

	//TODO: This should actually save the real entity object.
	localEntities[event.id] = {};
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
							// Massage the message into looking like an event.
							reply.type = "full";

							// Process it like a normal event
							handleIncomingEvent(reply);

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

					//TODO: Handle reparenting the camera here.
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
