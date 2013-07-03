#ifndef CAMERANODEOBJECT_H
#define CAMERANODEOBJECT_H

#include <QtCore/QObject>

#include <Horde3D.h>


class CameraNodeObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal yaw READ yaw WRITE setYaw)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)

public:
    explicit CameraNodeObject(H3DNode cam, QObject *parent = 0);

    H3DNode camera() const { return m_camera; }

    qreal yaw() const { return m_yaw; }
    qreal pitch() const { return m_pitch; }
    qreal zoom() const { return m_zoom; }

    void setYaw(qreal y);
    void setPitch(qreal p);
    void setZoom(qreal z);

private:
    void updateRotation();

    H3DNode m_camera;

    qreal m_yaw;
    qreal m_pitch;
    qreal m_zoom;
};

#endif // CAMERANODEOBJECT_H
