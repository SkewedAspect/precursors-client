#include <cmath>

#include <QtGui/QVector4D>

#include <Horde3DUtils.h>

#include "entity.h"
#include "math3d.h"


const qreal radiansPerDegree = 180 / M_PI;

QHash<H3DNode, Entity*> Entity::entities;
QList<Entity*> Entity::scheduledOnceEntities;
QList<Entity*> Entity::scheduledRepeatingEntities;


Entity::Entity() :
	QObject(), _node(0), scheduledOnce(false), scheduledRepeating(false),
	_parentChanged(false), _transformChanged(false), _awaitingRemove(false),
	_logger(PLogManager::getLogger("entity")), _mgr(Horde3DManager::instance())
{
	throw new std::exception;
} // end Entity

Entity::Entity(H3DNode node, QObject *parent) :
	QObject(parent), _node(node), scheduledOnce(false), scheduledRepeating(false),
	_parentChanged(false), _transformChanged(false), _awaitingRemove(false),
	_logger(PLogManager::getLogger("entity")), _mgr(Horde3DManager::instance())
{
	const float* matData;
	h3dGetNodeTransMats(node, &matData, NULL);
	_trans = QMatrix4x4(matData);

	float pitch, heading, roll;
	h3dGetNodeTransform(node,
			NULL, NULL, NULL,
			&pitch, &heading, &roll,
			NULL, NULL, NULL
			);
	rotateHeading(heading);
	rotatePitch(pitch);
	rotateRoll(roll);
} // end Entity


Entity::Flags Entity::flags() const
{
	return (Entity::Flags) h3dGetNodeFlags(_node);
} // end flags

QQuaternion Entity::orientation() const
{
	return _orientation;
} // end orientation

QVector3D Entity::pos() const
{
	return _trans.row(3).toVector3D();
} // end pos

Entity* Entity::parent() const
{
	return _parent;
} // end parent


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

void Entity::setFlags(Flags flags)
{
	h3dSetNodeFlags(_node, flags, false);
	emit flagsChanged(flags);
} // end setFlags

void Entity::setOrientation(QQuaternion orientation)
{
	// Reset transformation matrix, preserving position only.
	QVector4D translation = _trans.column(3);
	_trans.setToIdentity();
	_trans.setColumn(3, translation);

	// Rotate to the new orientation.
	rotate(orientation);
	_orientation = orientation;
} // end setOrientation

void Entity::setPos(QVector3D pos)
{
	setPos(pos.x(), pos.y(), pos.z());
} // end setPos

void Entity::setParent(Entity* parent)
{
	if(_parent != parent)
	{
		_parentChanged = true;
		_parent = parent;
		emit parentChanged(parent);
		scheduleOnce();
	} // end if
} // end setParent

void Entity::setFlagsRecursive(Flags flags)
{
	h3dSetNodeFlags(_node, flags, true);
	//FIXME: Theoretically, this also modifies the flags for all child nodes; descendant entities should fire their
	// flagsChanged signal here!
	emit flagsChanged(flags);
} // end setFlagsRecursive


void Entity::setPos(qreal x, qreal y, qreal z)
{
	_transformChanged = true;

	_trans(0, 3) = x;
	_trans(1, 3) = y;
	_trans(2, 3) = z;

	emit posChanged();
	scheduleOnce();
} // end setPos

void Entity::rotate(QQuaternion orientation)
{
	_transformChanged = true;

	_orientation *= orientation;
	_trans.rotate(orientation);
	emit orientationChanged();

	scheduleOnce();
} // end rotate

void Entity::rotateHeading(qreal heading)
{
	rotate(QQuaternion::fromAxisAndAngle(0, 1, 0, heading));
} // end rotateHeading

void Entity::rotatePitch(qreal pitch)
{
	rotate(QQuaternion::fromAxisAndAngle(1, 0, 0, pitch));
} // end rotatePitch

void Entity::rotateRoll(qreal roll)
{
	rotate(QQuaternion::fromAxisAndAngle(0, 0, 1, roll));
} // end rotateRoll


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
} // end findChildEntities

bool Entity::contains(Entity* other)
{
	return Entity::contains(_node, other->node());
} // end contains

bool Entity::contains(H3DNode other)
{
	return Entity::contains(_node, other);
} // end contains

bool Entity::isDescendantOf(Entity* other)
{
	return Entity::contains(other->node(), _node);
} // end isDescendantOf

bool Entity::isDescendantOf(H3DNode other)
{
	return Entity::contains(other, _node);
} // end contains


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
		_mgr.addSkybox(scenePath, skybox);
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
	_awaitingRemove = true;
	scheduleOnce();
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
	if(_awaitingRemove)
	{
		_logger.debug(QString("%1: Removing...").arg(toString()));
		_awaitingRemove = false;

		QList<Entity*> found = findChildEntities();

		while(!found.isEmpty())
		{
			found.takeFirst()->deleteLater();
		} // end while

		h3dRemoveNode(_node);
		deleteLater();
	}
	else
	{
		if(_parentChanged)
		{
			_logger.debug(QString("%1: Applying new parent...").arg(toString()));
			_parentChanged = false;

			if(_parent == NULL)
			{
				if(!h3dSetNodeParent(_node, _mgr.root()->node()))
				{
					_logger.error(QString("Failed to unset %1's parent!").arg(toString()));
				} // end if
			}
			else
			{
				if(!h3dSetNodeParent(_node, _parent->node()))
				{
					_logger.error(QString("Failed to set %1's parent to %2!")
							.arg(toString())
							.arg(_parent->toString())
							);
				} // end if
			} // end if
		} // end if

		if(_transformChanged)
		{
			//_logger.debug(QString("%1: Applying new transform...").arg(toString()));
			_transformChanged = false;

			h3dSetNodeTransMat(_node, _trans.data());
		} // end if
	} // end if

	if(scheduledOnce)
	{
		scheduledOnce = false;
	} // end if
} // end apply


QString Entity::toString() const
{
	QString entityPath = h3dGetNodeParamStr(_node, H3DNodeParams::NameStr);

	H3DNode parentNode = h3dGetNodeParent(_node);
	while(parentNode)
	{
		QString parentNodeName = h3dGetNodeParamStr(parentNode, H3DNodeParams::NameStr);
		entityPath = parentNodeName + "/" + entityPath;

		parentNode = h3dGetNodeParent(parentNode);
	} // end while

	return QString("<Entity %1: %2>").arg(int(_node)).arg(entityPath);
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

bool Entity::contains(H3DNode thisNode, H3DNode otherNode)
{
	H3DNode parentNode = h3dGetNodeParent(otherNode);
	while(parentNode)
	{
		if(parentNode == thisNode)
		{
			return true;
		} // end if

		parentNode = h3dGetNodeParent(parentNode);
	} // end while

	return false;
} // end isDescendantOf
