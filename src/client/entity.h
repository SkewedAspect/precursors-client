#ifndef ENTITY_H
#define ENTITY_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QMatrix4x4>
#include <QtGui/QQuaternion>
#include <QtGui/QVector3D>

#include <Horde3D.h>

#include "plogging/plogging.h"

#include "horde3dmanager.h"


class Entity : public QObject
{
	Q_OBJECT

	Q_PROPERTY(Flags flags READ flags WRITE setFlags NOTIFY flagsChanged)
	Q_PROPERTY(QQuaternion orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
	Q_PROPERTY(qreal heading READ heading NOTIFY orientationChanged)
	Q_PROPERTY(qreal pitch READ pitch NOTIFY orientationChanged)
	Q_PROPERTY(qreal roll READ roll NOTIFY orientationChanged)
	Q_PROPERTY(QVector3D pos READ pos WRITE setPos NOTIFY posChanged)
	Q_PROPERTY(Entity* parent READ parent WRITE setParent NOTIFY parentChanged)
	Q_PROPERTY(QVariantMap state MEMBER _state NOTIFY stateChanged)

	Q_ENUMS(Flags)

public:
	enum Flags
	{
		EF_NO_DRAW        = H3DNodeFlags::NoDraw,
		EF_NO_CAST_SHADOW = H3DNodeFlags::NoCastShadow,
		EF_NO_RAY_QUERY   = H3DNodeFlags::NoRayQuery,
		EF_INACTIVE       = H3DNodeFlags::Inactive
	}; // end Flags

	Entity();

	H3DNode node() const { return _node; }

	Flags flags() const;
	QQuaternion orientation() const;
	qreal heading() const;
	qreal pitch() const;
	qreal roll() const;
	QVector3D pos() const;
	Entity* parent() const;

	void setFlags(Flags flags);
	void setOrientation(QQuaternion orientation);
	void setPos(QVector3D pos);
	void setParent(Entity* parent);

	void setFlagsRecursive(Flags flags);

	Q_INVOKABLE void setPos(qreal x, qreal y, qreal z);

	Q_INVOKABLE void rotate(QQuaternion orientation);
	Q_INVOKABLE void rotateHeading(qreal heading);
	Q_INVOKABLE void rotatePitch(qreal pitch);
	Q_INVOKABLE void rotateRoll(qreal roll);

	Q_INVOKABLE void setState(QString key, QVariant value);
	Q_INVOKABLE void updateState(QVariantMap delta);
	Q_INVOKABLE QVariant getState(QString key, QVariant defaultValue);

	Q_INVOKABLE QList<Entity*> find(QString childName);
	Q_INVOKABLE QList<Entity*> findChildEntities();
	Q_INVOKABLE bool contains(Entity* other);
	Q_INVOKABLE bool contains(H3DNode other);
	Q_INVOKABLE bool isDescendantOf(Entity* other);
	Q_INVOKABLE bool isDescendantOf(H3DNode other);

	Q_INVOKABLE Entity* newCamera(QString cameraName, QString pipelineName = QString());
	Q_INVOKABLE Entity* newGroup(QString groupName);
	Q_INVOKABLE Entity* loadModel(QString scenePath, int flags = 0);
	Q_INVOKABLE Entity* loadScene(QString scenePath, int flags = 0);
	Q_INVOKABLE Entity* loadEntityFromRes(H3DResTypes::List type, QString path, int flags = 0);

	Q_INVOKABLE void remove();

	Q_INVOKABLE void scheduleOnce();
	Q_INVOKABLE void scheduleRepeating();
	Q_INVOKABLE void stopRepeating();

	Q_INVOKABLE static void runScheduled();
	Q_INVOKABLE static Entity* getEntity(H3DNode node, bool createIfMissing = true);
	Q_INVOKABLE static bool hasEntity(H3DNode node);
	Q_INVOKABLE static bool contains(H3DNode thisNode, H3DNode otherNode);

	void apply();

	Q_INVOKABLE QString toString() const;

signals:
	void flagsChanged(Flags flags);
	void orientationChanged();
	void posChanged();
	void parentChanged(Entity* parent);
	void stateChanged();

protected:
	explicit Entity(H3DNode node, QObject *parent = 0);

private:
	H3DNode _node;
	Entity* _parent;

	QVariantMap _state;

	QMatrix4x4 _trans;
	QQuaternion _orientation;

	bool scheduledOnce;
	bool scheduledRepeating;
	bool _parentChanged;
	bool _transformChanged;
	bool _awaitingRemove;

	PLogger& _logger;
	Horde3DManager& _mgr;

	static QHash<H3DNode, Entity*> entities;
	static QList<Entity*> scheduledOnceEntities;
	static QList<Entity*> scheduledRepeatingEntities;
}; // end Entity

#endif // ENTITY_H
