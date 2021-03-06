#ifndef HORDE3DITEM_H
#define HORDE3DITEM_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QSGGeometry>
#include <QtQuick/QSGTextureMaterial>
#include <QtCore/QPropertyAnimation>

#include <Horde3D.h>

#include "horde3ditemanim.h"


class QSGSimpleTextureNode;
class QOpenGLFramebufferObject;

class CameraNodeObject;


class Horde3DItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject* camera READ camera)

public:
    Horde3DItem(QQuickItem* parent = 0);
    ~Horde3DItem();

    QObject* camera() const { return m_cameraQObject; }

    void setAAEnabled(bool enable);

    void updateFBO();

    void init();

    void saveH3DState();
    bool restoreH3DState();

signals:
    void initFinished();

protected slots:
    void onInitFinished();
    void onBeforeRendering();

protected:
    virtual QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data);

    virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    virtual void timerEvent(QTimerEvent* event);

private:
    void printHordeMessages();
    void renderHorde();

    int m_timerID;

    QObject* m_cameraQObject;

    QSGSimpleTextureNode* m_node;
    QOpenGLFramebufferObject* m_fbo;

    QSGTexture* m_texture;

    // Qt's OpenGL context
    QOpenGLContext* m_qtContext;
    // The context to be used by Horde3D
    QOpenGLContext* m_h3dContext;

    int m_samples;
    bool m_AAEnabled;
    QSize m_size;
    float m_animTime;

    Horde3DItemAnimation m_animation;
    H3DNode m_knight;
    H3DNode m_ares;
    H3DNode m_camera;

    CameraNodeObject* m_cameraObject;

    bool m_initialized;
    bool m_dirtyFBO;
}; // end Horde3DItem

#endif // HORDE3DITEM_H
