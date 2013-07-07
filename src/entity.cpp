#include "entity.h"


QHash<H3DNode, Entity*> Entity::entities;
QList<Entity*> Entity::scheduledOnceEntities;
QList<Entity*> Entity::scheduledRepeatingEntities;


Entity::Entity() :
    QObject(), logger(PLogManager::getLogger("entity")), scheduledOnce(false), scheduledRepeating(false)
{
	throw new std::exception;
} // end Entity

Entity::Entity(H3DNode node, QObject *parent) :
    QObject(parent), _node(node), logger(PLogManager::getLogger("entity"))
{
	float x, y, z;

	h3dGetNodeTransform(node,
			&x, &y, &z,
			&_pitch, &_heading, &_roll,
			NULL, NULL, NULL);

	_pos = QVector3D(x, y, z);
} // end Entity

qreal Entity::heading() const
{
	return _heading;
} // end heading

qreal Entity::pitch() const
{
	return _pitch;
} // end pitch

qreal Entity::roll() const
{
	return _roll;
} // end roll

void Entity::setHeading(qreal heading)
{
	_heading = heading;
	scheduleOnce();
} // end setHeading

void Entity::setPitch(qreal pitch)
{
	_pitch = pitch;
	scheduleOnce();
} // end setPitch

void Entity::setRoll(qreal roll)
{
	_roll = roll;
	scheduleOnce();
} // end setRoll

void Entity::changeHeading(qreal dY)
{
	_heading += dY;
	scheduleOnce();
} // end changeHeading

void Entity::changePitch(qreal dP)
{
	_pitch += dP;
	scheduleOnce();
} // end changePitch

void Entity::changeRoll(qreal dR)
{
	_roll += dR;
	scheduleOnce();
} // end changeRoll

void Entity::scheduleOnce()
{
	if(!scheduledOnce && !scheduledRepeating)
	{
		scheduledOnceEntities.append(this);
	} // end if

	scheduledOnce = true;
} // end scheduleOnce

void Entity::scheduleRepeating()
{
	if(!scheduledRepeating)
	{
		scheduledRepeatingEntities.append(this);

		scheduledRepeating = true;
	} // end if
} // end scheduleRepeating

void Entity::stopRepeating()
{
	scheduledRepeatingEntities.removeAll(this);
} // end scheduleRepeating

void Entity::apply()
{
	h3dSetNodeTransform(_node,
			_pos.x(), _pos.y(), _pos.z(),
			_pitch, _heading, _roll,
			1, 1, 1);

	if(scheduledOnce)
	{
		scheduledOnce = false;
	} // end if
} // end apply


// Static Methods //

void Entity::runScheduled()
{
	while(!scheduledOnceEntities.isEmpty())
	{
		scheduledOnceEntities.takeFirst()->apply();
	} // end while

	for(int i = 0; i < scheduledRepeatingEntities.size(); i++)
	{
		scheduledRepeatingEntities.at(i)->apply();
	} // end for
} // end runScheduled

Entity* Entity::getEntity(H3DNode node)
{
	if(!entities.contains(node))
	{
		entities[node] = new Entity(node);
	} // end if

	return entities[node];
} // end getEntity
