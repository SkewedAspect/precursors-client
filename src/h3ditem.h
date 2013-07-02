#ifndef H3DITEM_H
#define H3DITEM_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QSGGeometry>
#include <QtQuick/QSGTextureMaterial>
#include <QtCore/QPropertyAnimation>


class QSGSimpleTextureNode;
class QGLFramebufferObject;

class CameraNodeObject;


class Horde3DItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *camera READ camera)

public:
    Horde3DItem(QQuickItem *parent = 0);
    ~Horde3DItem();

    QObject *camera() const { return m_cameraQObject; }

    void setSize(const QSize &size);
    QSize size() const { return m_size; }

    void setAAEnabled(bool enable);

    void updateFBO();

    void init();

    void saveH3DState();
    void restoreH3DState();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_timerID;

    QObject *m_cameraQObject;

    QSGSimpleTextureNode *m_node;

    QSGTextureMaterial m_material;
    QSGOpaqueTextureMaterial m_materialO;
    QSGGeometry m_geometry;
    QSGTexture *m_texture;

    int m_samples;
    bool m_AAEnabled;
    QSize m_size;

	bool m_firstRun;
	bool m_valid;
	QGLFramebufferObject *m_fbo;
	HordeViewportAnimation m_animation;
	H3DNode m_camera;

    CameraNodeObject *m_cameraObject;

    bool m_initialized;
    bool m_dirtyFBO;
};

#endif // H3DITEM_H
