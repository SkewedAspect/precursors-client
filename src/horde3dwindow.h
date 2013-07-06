#ifndef HORDE3DWINDOW_H
#define HORDE3DWINDOW_H

#include <QtQuick/QQuickWindow>
#include <QtCore/QTime>

#include <Horde3D.h>

#include "h3ditemanim.h"


class QSGSimpleTextureNode;
class QOpenGLFramebufferObject;

class CameraNodeObject;


class Horde3DWindow : public QQuickWindow
{
	Q_OBJECT

	Q_PROPERTY(QObject* camera READ camera)
	Q_PROPERTY(float fps READ fps NOTIFY fpsChanged)

public:
	Horde3DWindow(QWindow* parent = 0);
	~Horde3DWindow();

	QObject* camera() const { return m_cameraQObject; }
	float fps() const { return lastFPS; }

	void saveQtState();
	void restoreQtState();
	void restoreH3DState();
	void saveH3DState();

signals:
	void initFinished();
	void fpsChanged(const float& newFPS);

protected slots:
	void onInitFinished();
	void onBeforeRendering();

protected:
	virtual void resizeEvent(QResizeEvent* event);

	virtual void timerEvent(QTimerEvent* event);

private:
	void updateView();

	void init();

	void printHordeMessages();
	void renderHorde();

	// Qt's OpenGL context
	QOpenGLContext* m_qtContext;
	// The context to be used by Horde3D
	QOpenGLContext* m_h3dContext;

	QTime lastFrameStart;
	float lastFPS;

	int m_samples;
	QSize m_size;
	float m_animTime;

	H3DNode m_knight;
	H3DNode m_ares;
	H3DNode m_camera;

	CameraNodeObject* m_cameraObject;
	QObject* m_cameraQObject;

	bool m_initialized;
	bool m_dirtyView;
}; // end Horde3DWindow

#endif // HORDE3DWINDOW_H
