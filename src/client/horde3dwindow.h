#ifndef HORDE3DWINDOW_H
#define HORDE3DWINDOW_H

#include <QtQuick/QQuickWindow>
#include <QtCore/QTime>

#include <Horde3D.h>

#include "horde3dmanager.h"
#include "horde3ditemanim.h"


class QSGSimpleTextureNode;
class QOpenGLFramebufferObject;

class Entity;


class Horde3DWindow : public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(Entity* camera READ camera NOTIFY cameraChanged)
    Q_PROPERTY(Entity* camDolly READ camDolly)
    Q_PROPERTY(float fps READ fps NOTIFY fpsChanged)

public:
    Horde3DWindow(QWindow* parent = 0);
    ~Horde3DWindow();

    Entity* camera() const;
    Entity* camDolly() const;
    float fps() const;

    void saveQtState();
    void restoreQtState();
    void restoreH3DState();
    void saveH3DState();

signals:
    void cameraChanged(const Entity* camera);
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

    void renderHorde();

    // Qt's OpenGL context
    QOpenGLContext* _qtContext;
    // The context to be used by Horde3D
    QOpenGLContext* _h3dContext;

    QTime lastFrameStart;
    float lastFrameTime;
    float lastFPS;
    float _shipRot;

    QSize _size;

    Entity* _camDolly;
    Entity* _camera;

    Horde3DManager& _mgr;

    bool _initialized;
    bool _dirtyView;
}; // end Horde3DWindow

#endif // HORDE3DWINDOW_H
