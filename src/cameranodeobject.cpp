#include "cameranodeobject.h"


CameraNodeObject::CameraNodeObject(H3DNode cam, QObject *parent) :
    QObject(parent), m_camera(cam), m_heading(0), m_pitch(0), m_zoom(3)
{
    updateRotation();
} // end CameraNodeObject

void CameraNodeObject::setHeading(qreal y)
{
	m_heading = y;
	updateRotation();
} // end setHeading

void CameraNodeObject::setPitch(qreal p)
{
	m_pitch = p;
	updateRotation();
} // end setPitch

void CameraNodeObject::setZoom(qreal z)
{
    m_zoom = z;
    updateRotation();
} // end setZoom

void CameraNodeObject::updateRotation()
{
	h3dSetNodeTransform(m_camera,
			0, 0, 1000 - (100 * m_zoom), // translation
			m_pitch, m_heading, 0,       // orientation
			1, 1, 1                      // scale
			);
} // end updateRotation
