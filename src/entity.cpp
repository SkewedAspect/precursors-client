#include "entity.h"

#include <cmath>

#include <QtGui/QQuaternion>


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

QQuaternion Entity::eulerToQuat(qreal heading, qreal pitch, qreal roll)
{
    qreal hy = heading / 2;
    qreal hp = pitch / 2;
    qreal hr = roll / 2;

    return QQuaternion(
            cos(hp) * cos(hr) * cos(hy) + sin(hp) * sin(hr) * sin(hy),
            sin(hp) * cos(hr) * cos(hy) - cos(hp) * sin(hr) * sin(hy),
            cos(hp) * sin(hr) * cos(hy) + sin(hp) * cos(hr) * sin(hy),
            cos(hp) * cos(hr) * sin(hy) - sin(hp) * sin(hr) * cos(hy)
            );
} // end eulerToQuat

qreal Entity::quatToHeading(QQuaternion quat)
{
	qreal halfSinePitch = quat.x() * quat.y() + quat.z() * quat.scalar();

	if(halfSinePitch == 0.5) // north pole
	{
		return 2 * atan2(quat.x(), quat.scalar());
	}
	else if(halfSinePitch == -0.5) // south pole
	{
		return -2 * atan2(quat.x(), quat.scalar());
	}
	else
	{
		return atan2(2 * quat.y() * quat.scalar() - 2 * quat.x() * quat.z(),
				1 - 2 * pow(quat.y(), 2) - 2 * pow(quat.z(), 2));
	} // end if
} // end quatToHeading

qreal Entity::quatToPitch(QQuaternion quat)
{
	return asin(2 * quat.x() * quat.y() + 2 * quat.z() * quat.scalar());
} // end quatToPitch

qreal Entity::quatToRoll(QQuaternion quat)
{
	qreal halfSinePitch = quat.x() * quat.y() + quat.z() * quat.scalar();

	if(halfSinePitch == 0.5) // north pole
	{
		return 0;
	}
	else if(halfSinePitch == -0.5) // south pole
	{
		return 0;
	}
	else
	{
		return atan2(2 * quat.x() * quat.scalar() - 2 * quat.y() * quat.z(),
				1 - 2 * pow(quat.x(), 2) - 2 * pow(quat.z(), 2));
	} // end if
} // end quatToRoll

qreal Entity::matrixToHeading(QMatrix4x4 mat)
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

void Entity::matrixToEuler(QMatrix4x4 mat, qreal *heading, qreal *pitch, qreal *roll)
{
	if(mat(1, 2) > 0.9999999998 || mat(1, 2) < -0.9999999998)
	{
		*heading = atan2(-mat(2, 0), mat(0, 0));
		*roll = 0;
	}
	else
	{
		*heading = atan2(mat(0, 2), mat(2, 2));
		*roll = atan2(mat(1, 0), mat(1, 1));
	} // end if

	*pitch = asin(mat(1, 2));
} // end eulerToQuat
