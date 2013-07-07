#ifndef ENTITY_H
#define ENTITY_H

#include <QtCore/QObject>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>

#include <Horde3D.h>

#include "plogging/plogging.h"


class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal yaw READ yaw WRITE setYaw)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal roll READ roll WRITE setRoll)

public:
    Entity();

    H3DNode node() const { return _node; }

    qreal yaw() const;
    qreal pitch() const;
    qreal roll() const;

    void setYaw(qreal y);
    void setPitch(qreal p);
    void setRoll(qreal r);

	Q_INVOKABLE void changeYaw(qreal dY);
	Q_INVOKABLE void changePitch(qreal dP);
	Q_INVOKABLE void changeRoll(qreal dR);

	Q_INVOKABLE void schedule();

	Q_INVOKABLE static Entity* getEntity(H3DNode node);
	Q_INVOKABLE static void runScheduled();

    void apply();

	static QQuaternion eulerToQuat(qreal yaw, qreal pitch, qreal roll);
	static void matrixToEuler(QMatrix4x4 mat, qreal *yaw, qreal *pitch, qreal *roll);

	static qreal matrixToYaw(QMatrix4x4 mat);
	static qreal matrixToPitch(QMatrix4x4 mat);
	static qreal matrixToRoll(QMatrix4x4 mat);

protected:
    explicit Entity(H3DNode node, QObject *parent = 0);

private:
    H3DNode _node;

	QVector3D _pos;
	float _heading, _pitch, _roll;

	PLogger& logger;

	static QHash<H3DNode, Entity*> entities;
	static QList<Entity*> scheduledEntities;
}; // end Entity

#endif // ENTITY_H
