#include "entity.h"

#include <cmath>

#include <QtGui/QQuaternion>


QHash<H3DNode, Entity*> Entity::entities;
QList<Entity*> Entity::scheduledEntities;


Entity::Entity() :
    QObject(), logger(PLogManager::getLogger("entity"))
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

qreal Entity::yaw() const
{
	return _heading;
} // end yaw

qreal Entity::pitch() const
{
	return _pitch;
} // end pitch

qreal Entity::roll() const
{
	return _roll;
} // end roll

void Entity::setYaw(qreal heading)
{
	_heading = heading;
	schedule();
} // end setYaw

void Entity::setPitch(qreal pitch)
{
	_pitch = pitch;
	schedule();
} // end setPitch

void Entity::setRoll(qreal roll)
{
	_roll = roll;
	schedule();
} // end setRoll

void Entity::changeYaw(qreal dY)
{
	_heading += dY;
	schedule();
} // end changeYaw

void Entity::changePitch(qreal dP)
{
	_pitch += dP;
	schedule();
} // end changePitch

void Entity::changeRoll(qreal dR)
{
	_roll += dR;
	schedule();
} // end changeRoll

void Entity::schedule()
{
	scheduledEntities.append(this);
} // end schedule

void Entity::apply()
{
	h3dSetNodeTransform(_node,
			_pos.x(), _pos.y(), _pos.z(),
			_pitch, _heading, _roll,
			1, 1, 1);
} // end apply

void Entity::runScheduled()
{
	while(!scheduledEntities.isEmpty())
	{
		scheduledEntities.takeFirst()->apply();
	} // end while
} // end runScheduled

Entity* Entity::getEntity(H3DNode node)
{
	if(!entities.contains(node))
	{
		entities[node] = new Entity(node);
	} // end if

	return entities[node];
} // end getEntity

QQuaternion Entity::eulerToQuat(qreal yaw, qreal pitch, qreal roll)
{
    qreal hy = yaw / 2;
    qreal hp = pitch / 2;
    qreal hr = roll / 2;

    return QQuaternion(
            cos(hp) * cos(hr) * cos(hy) + sin(hp) * sin(hr) * sin(hy),
            sin(hp) * cos(hr) * cos(hy) - cos(hp) * sin(hr) * sin(hy),
            cos(hp) * sin(hr) * cos(hy) + sin(hp) * cos(hr) * sin(hy),
            cos(hp) * cos(hr) * sin(hy) - sin(hp) * sin(hr) * cos(hy)
            );
} // end eulerToQuat

qreal Entity::matrixToYaw(QMatrix4x4 mat)
{
	if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
	{
		return atan2(-mat(2, 0), mat(0, 0));
	}
	else
	{
		return atan2(mat(0, 2), mat(2, 2));
	} // end if
} // end eulerToQuat

qreal Entity::matrixToPitch(QMatrix4x4 mat)
{
	return asin(mat(1, 2));
} // end eulerToQuat

qreal Entity::matrixToRoll(QMatrix4x4 mat)
{
	if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
	{
		return 0;
	}
	else
	{
		return atan2(mat(1, 0), mat(1, 1));
	} // end if
} // end eulerToQuat

void Entity::matrixToEuler(QMatrix4x4 mat, qreal *yaw, qreal *pitch, qreal *roll)
{
	if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
	{
		*yaw = atan2(-mat(2, 0), mat(0, 0));
		*roll = 0;
	}
	else
	{
		*yaw = atan2(mat(0, 2), mat(2, 2));
		*roll = atan2(mat(1, 0), mat(1, 1));
	} // end if

	*pitch = asin(mat(1, 2));
} // end eulerToQuat
