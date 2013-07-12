#ifndef ENTITY_H
#define ENTITY_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QVector3D>

#include <Horde3D.h>

#include "plogging/plogging.h"

#include "horde3dmanager.h"


class Entity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal heading READ heading WRITE setHeading)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal roll READ roll WRITE setRoll)
	Q_PROPERTY(QVector3D pos MEMBER _pos NOTIFY posChanged)
	Q_PROPERTY(QVariantMap state MEMBER _state NOTIFY stateChanged)

public:
    Entity();

    H3DNode node() const { return _node; }

    qreal heading() const;
    qreal pitch() const;
    qreal roll() const;

    void setHeading(qreal y);
    void setPitch(qreal p);
    void setRoll(qreal r);

    Q_INVOKABLE void setPos(qreal x, qreal y, qreal z);

	Q_INVOKABLE void setState(QString key, QVariant value);
	Q_INVOKABLE void updateState(QVariantMap delta);
	Q_INVOKABLE QVariant getState(QString key, QVariant defaultValue);

    Q_INVOKABLE void changeHeading(qreal dY);
    Q_INVOKABLE void changePitch(qreal dP);
    Q_INVOKABLE void changeRoll(qreal dR);

	Q_INVOKABLE QList<Entity*> find(QString childName);

    Q_INVOKABLE Entity* newCamera(QString cameraName, QString pipelineName = QString());
    Q_INVOKABLE Entity* newGroup(QString groupName);
    Q_INVOKABLE Entity* loadModel(QString scenePath, int flags = 0);
    Q_INVOKABLE Entity* loadScene(QString scenePath, int flags = 0);
    Q_INVOKABLE Entity* loadEntityFromRes(H3DResTypes::List type, QString path, int flags = 0);

    Q_INVOKABLE void scheduleOnce();
    Q_INVOKABLE void scheduleRepeating();
    Q_INVOKABLE void stopRepeating();

    Q_INVOKABLE static Entity* getEntity(H3DNode node);
    Q_INVOKABLE static void runScheduled();

    void apply();

signals:
	void stateChanged();
	void posChanged();

protected:
    explicit Entity(H3DNode node, QObject *parent = 0);

private:
    H3DNode _node;

	QVariantMap _state;

    QVector3D _pos;
    float _heading, _pitch, _roll;

    bool scheduledOnce;
    bool scheduledRepeating;

    PLogger& _logger;
    Horde3DManager& _mgr;

    static QHash<H3DNode, Entity*> entities;
    static QList<Entity*> scheduledOnceEntities;
    static QList<Entity*> scheduledRepeatingEntities;
}; // end Entity

#endif // ENTITY_H
