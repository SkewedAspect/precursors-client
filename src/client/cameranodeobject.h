#ifndef CAMERANODEOBJECT_H
#define CAMERANODEOBJECT_H

#include <QtCore/QObject>

#include <Horde3D.h>


class CameraNodeObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal heading READ heading WRITE setHeading)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)

public:
    explicit CameraNodeObject(H3DNode cam, QObject *parent = 0);

    H3DNode camera() const { return m_camera; }

    qreal heading() const { return m_heading; }
    qreal pitch() const { return m_pitch; }
    qreal zoom() const { return m_zoom; }

    void setHeading(qreal y);
    void setPitch(qreal p);
    void setZoom(qreal z);

    void updateRotation();

private:
    H3DNode m_camera;

    qreal m_heading;
    qreal m_pitch;
    qreal m_zoom;
};

#endif // CAMERANODEOBJECT_H
