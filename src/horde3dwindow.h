#ifndef HORDE3DWINDOW_H
#define HORDE3DWINDOW_H

#include <QtQuick/QQuickWindow>
#include <QtCore/QTime>

#include <Horde3D.h>

#include "h3ditemanim.h"


class QSGSimpleTextureNode;
class QOpenGLFramebufferObject;

class Entity;


class Horde3DWindow : public QQuickWindow
{
	Q_OBJECT

	Q_PROPERTY(Entity* camera READ camera)
	Q_PROPERTY(Entity* camDolly READ camDolly)
	Q_PROPERTY(float fps READ fps NOTIFY fpsChanged)

public:
	Horde3DWindow(QWindow* parent = 0);
	~Horde3DWindow();

	Entity* camera() const { return _cameraEnt; }
	Entity* camDolly() const { return _camDollyEnt; }
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
	QOpenGLContext* _qtContext;
	// The context to be used by Horde3D
	QOpenGLContext* _h3dContext;

	QTime lastFrameStart;
	float lastFPS;

	int _samples;
	QSize _size;
	float _animTime;

	H3DNode _ares;

	H3DNode _camDolly;
	Entity* _camDollyEnt;

	H3DNode _camera;
	Entity* _cameraEnt;

	bool _initialized;
	bool _dirtyView;
}; // end Horde3DWindow

#endif // HORDE3DWINDOW_H
