#include <Horde3DUtils.h>

#include "entity.h"


QHash<H3DNode, Entity*> Entity::entities;
QList<Entity*> Entity::scheduledOnceEntities;
QList<Entity*> Entity::scheduledRepeatingEntities;


Entity::Entity() :
	QObject(), _node(0), scheduledOnce(false), scheduledRepeating(false),
	_logger(PLogManager::getLogger("entity")), _mgr(Horde3DManager::instance())
{
	throw new std::exception;
} // end Entity

Entity::Entity(H3DNode node, QObject *parent) :
	QObject(parent), _node(node), scheduledOnce(false), scheduledRepeating(false),
	_logger(PLogManager::getLogger("entity")), _mgr(Horde3DManager::instance())
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

QVector3D Entity::pos() const
{
	return _pos;
} // end pos

Entity* Entity::parent() const
{
	return _parent;
} // end parent

void Entity::setPos(qreal x, qreal y, qreal z)
{
	setPos(QVector3D(x, y, z));
} // end setPos

void Entity::setState(QString key, QVariant value)
{
	_state[key] = value;

	emit stateChanged();
} // end setState

void Entity::updateState(QVariantMap delta)
{
	for(QVariantMap::iterator it = delta.begin(); it != delta.end(); )
	{
		_state.insert(it.key(), it.value());
		it++;
	} // end for

	emit stateChanged();
} // end updateState

QVariant Entity::getState(QString key, QVariant defaultValue)
{
	return _state.value(key, defaultValue);
} // end getState

void Entity::setHeading(qreal heading)
{
	_heading = heading;
	emit headingChanged(heading);
	scheduleOnce();
} // end setHeading

void Entity::setPitch(qreal pitch)
{
	_pitch = pitch;
	emit pitchChanged(pitch);
	scheduleOnce();
} // end setPitch

void Entity::setRoll(qreal roll)
{
	_roll = roll;
	emit rollChanged(roll);
	scheduleOnce();
} // end setRoll

void Entity::setPos(QVector3D pos)
{
	_pos = pos;
	emit posChanged(pos);
	scheduleOnce();
} // end setRoll

void Entity::setParent(Entity* parent)
{
	if(parent == NULL)
	{
		if(!h3dSetNodeParent(_node, _mgr.root()->node()))
		{
			_logger.error(QString("Failed to unset %1's parent!").arg(toString()));
			return;
		} // end if
	}
	else
	{
		if(!h3dSetNodeParent(_node, parent->node()))
		{
			_logger.error(QString("Failed to set %1's parent to %2!").arg(toString()).arg(parent->toString()));
			return;
		} // end if
	} // end if

	_parent = parent;
	emit parentChanged(parent);
} // end setRoll


QList<Entity*> Entity::find(QString childName)
{
	int numFound = h3dFindNodes(_node, childName.toUtf8().constData(), H3DNodeTypes::Undefined);

	QList<Entity*> found;
	for(int idx = 0; idx < numFound; idx++)
	{
		found.append(Entity::getEntity(h3dGetNodeFindResult(idx)));
	} // end for

	return found;
} // end find

QList<Entity*> Entity::findChildEntities()
{
	QList<Entity*> found;

	int numFound = h3dFindNodes(_node, "", H3DNodeTypes::Undefined);
	for(int idx = 0; idx < numFound; idx++)
	{
		Entity* child = Entity::getEntity(h3dGetNodeFindResult(idx), false);
		if(child)
		{
			found.append(child);
		} // end if
	} // end for

	return found;
} // end find


Entity* Entity::newGroup(QString groupName)
{
	H3DNode groupNode = h3dAddGroupNode(_node, groupName.toUtf8().constData());
	if(groupNode == 0)
	{
		_logger.error(QString("Failed to create group %1!").arg(groupName));
	} // end if

	return Entity::getEntity(groupNode);
} // end newGroup

Entity* Entity::newCamera(QString cameraName, QString pipelineName)
{
	H3DRes pipeline = _mgr.loadPipeline(pipelineName);
	if(pipeline == 0)
	{
		_logger.error(QString("Failed to load pipeline %1!").arg(pipelineName));
	} // end if

	H3DNode camera = h3dAddCameraNode(_node, cameraName.toUtf8().constData(), pipeline);
	if(camera == 0)
	{
		_logger.error(QString("Failed to create camera %1!").arg(cameraName));
	} // end if

	//FIXME: Get rid of this! (and replace it with getters/setters for pos, x, y, and z)
	h3dSetNodeTransform(camera,
			0, 0, 100,
			0, 0, 0,
			1, 1, 1
			);

	return Entity::getEntity(camera);
} // end newCamera

Entity* Entity::loadModel(QString modelPath, int flags)
{
	return loadEntityFromRes(H3DResTypes::SceneGraph, modelPath, flags);
} // end loadModel

Entity* Entity::loadScene(QString scenePath, int flags)
{
	Entity* scene = loadModel(scenePath, flags);

	// Apply NoCastShadow to all skyboxes.
	QList<Entity*> skyboxes = scene->find("skybox");
	while(!skyboxes.isEmpty())
	{
		Entity* skybox = skyboxes.takeFirst();
		h3dSetNodeFlags(skybox->node(), H3DNodeFlags::NoCastShadow | H3DNodeFlags::NoRayQuery, true);
		_mgr.addSkybox(skybox);
	} // end while

	return scene;
} // end loadScene

Entity* Entity::loadEntityFromRes(H3DResTypes::List type, QString path, int flags)
{
	H3DRes res = h3dAddResource(type, path.toUtf8().constData(), flags);
	if(res == 0)
	{
		_logger.error(QString("Failed to create resource handle for %1!").arg(path));
	} // end if

	if(!h3dIsResLoaded(res))
	{
		_logger.debug(QString("Loading resources from: %1").arg(_mgr.contentDirs()));
		if(!_mgr.loadNewResources())
		{
			_logger.error(QString("Failed to load resource %1!").arg(path));
			return NULL;
		} // end if
	} // end if

	H3DNode newNode = h3dAddNodes(_node, res);
	if(newNode == 0)
	{
		_logger.error(QString("Failed to create entity node for resource %1!").arg(path));
	} // end if

	return Entity::getEntity(newNode);
} // end loadEntityFromRes


void Entity::remove()
{
	QList<Entity*> found = findChildEntities();

	while(!found.isEmpty())
	{
		found.takeFirst()->remove();
	} // end while

	h3dRemoveNode(_node);
	deleteLater();
} // end remove


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


QString Entity::toString()
{
	return QString("<Entity %1>").arg(int(_node));
} // end toString


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

bool Entity::hasEntity(H3DNode node)
{
	return entities.contains(node);
} // end hasEntity

Entity* Entity::getEntity(H3DNode node, bool createIfMissing)
{
	if(!entities.contains(node))
	{
		if(createIfMissing)
		{
			entities[node] = new Entity(node);
		}
		else
		{
			return NULL;
		} // end if
	} // end if

	return entities[node];
} // end getEntity
