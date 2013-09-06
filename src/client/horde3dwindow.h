#ifndef HORDE3DWINDOW_H
#define HORDE3DWINDOW_H

#include <QtQuick/QQuickWindow>
#include <QtCore/QTime>

#include <Horde3D.h>

#include "controls/controlsmanager.h"

#include "horde3dmanager.h"
#include "horde3ditemanim.h"


#define CFG_VIEW_DIST "horde3d.viewDistance"
#define DEFAULT_VIEW_DIST 100000.0f


class QSGSimpleTextureNode;
class QOpenGLFramebufferObject;

class Entity;


class Horde3DWindow : public QQuickWindow
{
	Q_OBJECT

	Q_PROPERTY(Entity* camera READ camera NOTIFY cameraChanged)
	Q_PROPERTY(Entity* camDolly READ camDolly)
	Q_PROPERTY(float fps READ fps NOTIFY fpsChanged)
	Q_PROPERTY(int lastFrameTime READ lastFrameTime NOTIFY lastFrameTimeChanged)
	Q_PROPERTY(float maxViewDistance READ maxViewDistance WRITE setMaxViewDistance NOTIFY maxViewDistanceChanged)
	Q_PROPERTY(bool grabMouse READ grabMouse WRITE setGrabMouse NOTIFY grabMouseChanged)
	Q_PROPERTY(QPoint lastRecenterPos READ lastRecenterPos)

public:
	Horde3DWindow(QWindow* parent = 0);
	~Horde3DWindow();

	Entity* camera() const;
	Entity* camDolly() const;
	float fps() const;
	int lastFrameTime() const;
	float maxViewDistance() const;
	bool grabMouse() const;
	QPoint lastRecenterPos() const;

	void setMaxViewDistance(float maxViewDist);
	void setGrabMouse(bool grab);

	void saveQtState();
	void restoreQtState();
	void restoreH3DState();
	void saveH3DState();

signals:
	void cameraChanged(const Entity* camera);
	void fpsChanged(const float& newFPS);
	void lastFrameTimeChanged(const int& lastFrameTime);
	void maxViewDistanceChanged(float maxViewDist);
	void grabMouseChanged(bool grab);

	void keyPressed(QKeyEvent* event);
	void keyReleased(QKeyEvent* event);

	void mouseMoved(QMouseEvent* event, QPoint screenDelta);
	void mousePressed(QMouseEvent* event);
	void mouseReleased(QMouseEvent* event);
	void mouseWheel(QWheelEvent* event);

protected slots:
	void onInitFinished();
	void onBeforeRendering();

public slots:
	void onSceneChanged(const Entity* scene, QString sceneID);

protected:
	virtual void resizeEvent(QResizeEvent* event);

	virtual void timerEvent(QTimerEvent* event);

	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);

	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* ev);

private:
	void updateView();

	void init();

	void renderHorde();

	// Qt's OpenGL context
	QOpenGLContext* _qtContext;
	// The context to be used by Horde3D
	QOpenGLContext* _h3dContext;

	QTime lastFrameStart;
	int _lastFrameTime;
	float lastFPS;
	float _shipRot;
	bool _grabMouse;
	QPoint _lastRecenterPos;

	QSize _size;

	Entity* _camDolly;
	Entity* _camera;
	Entity* _skybox;
	QString _sceneID;

	bool _initialized;
	bool _dirtyView;

	PLogger& _logger;
	PSettingsManager& _settings;
	ControlsManager& _controls;
	Horde3DManager& _mgr;
}; // end Horde3DWindow

#endif // HORDE3DWINDOW_H
