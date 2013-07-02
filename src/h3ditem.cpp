#include "../config.h"
#include "h3ditem.h"
#include "cameranodeobject.h"

#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGSimpleTextureNode>


Horde3DItem::Horde3DItem(QQuickItem *parent) :
	QQuickItem(parent),
    m_timerID(0),
    m_node(0),
    m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4),
    m_texture(0),
    m_samples(0),
    m_AAEnabled(false),
    m_initialized(false),
    m_dirtyFBO(false)
{
    setFlag(ItemHasContents);
    setSmooth(false);

    startTimer(16);
} // end Horde3DItem

Horde3DItem::~Horde3DItem()
{
	//TODO: Cleanup!
} // end ~Horde3DItem

QSGNode *Horde3DItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData* data)
{
	Q_UNUSED(data);

    if(width() <= 0 || height() <= 0)
	{
        delete oldNode;
        return 0;
    } // end if

    m_node = static_cast<QSGSimpleTextureNode *>(oldNode);
    if(!m_node)
    {
        m_node = new QSGSimpleTextureNode();
        m_node->setFiltering(QSGTexture::Nearest);
        m_node->setMaterial(&m_material);
        m_node->setOpaqueMaterial(&m_materialO);
        m_node->setGeometry(&m_geometry);
        m_node->setFlag(QSGNode::UsePreprocess);
    } // end if
    m_node->setRect(boundingRect());

    setSize(QSize(width(), height()));
    setAAEnabled(smooth());

	//QPaintEngine::Type engineType = painter->paintEngine()->type();
	//if(engineType == QPaintEngine::OpenGL || engineType == QPaintEngine::OpenGL2)
	{
		//painter->beginNativePainting();

		if(!m_initialized)
		{
			init();
		} // end if

		if(m_dirtyFBO)
		{
			updateFBO();
		} // end if

		m_node->markDirty(QSGNode::DirtyMaterial);

		// TODO: Update scene.

		// Render Horde3D!
		restoreH3DState();

		// Create a camera, if we don't have one.
		if(m_camera)
		{
		} // end if

		// Render scene.
		h3dRender(m_camera);
		h3dFinalizeFrame();

		saveH3DState();

		// Render FBO to quad.
		glEnable(GL_TEXTURE_2D);
		m_fbo->drawTexture(QPointF(0, 0), m_fbo->texture());

		//painter->endNativePainting();
	} // end if

    m_node->setTexture(m_texture);

    return m_node;
} // end updatePaintNode

void Horde3DItem::timerEvent(QTimerEvent *)
{
    update();
} // end timerEvent

void Horde3DItem::saveH3DState()
{
	ctx->functions()->glDisable(GL_CULL_FACE);
	ctx->functions()->glDisable(GL_DEPTH_TEST);

    const QGLContext *ctx = QGLContext::currentContext();
    ctx->functions()->glPushAttrib(GL_ALL_ATTRIB_BITS);
	m_fbo->bind();
} // end saveH3DState

void Horde3DItem::restoreH3DState()
{
    const QGLContext *ctx = QGLContext::currentContext();
	m_fbo->release();
    ctx->functions()->glPopAttrib();
    ctx->functions()->glUseProgram(0);
} // end restoreH3DState

void Horde3DItem::updateFBO()
{
	if(m_fbo)
	{
		delete m_fbo;
	} // end if

	int deviceWidth = m_size.width();
	int deviceHeight = m_size.height();

	m_fbo = new QGLFramebufferObject(deviceWidth, deviceHeight);

	if(m_camera)
	{
		// Resize viewport
		h3dSetNodeParamI(m_camera, H3DCamera::ViewportXI, 0);
		h3dSetNodeParamI(m_camera, H3DCamera::ViewportYI, 0);
		h3dSetNodeParamI(m_camera, H3DCamera::ViewportWidthI, deviceWidth);
		h3dSetNodeParamI(m_camera, H3DCamera::ViewportHeightI, deviceHeight);

		// Set virtual camera parameters
		float aspectRatio = static_cast<float>(deviceWidth) / deviceHeight;
		h3dSetupCameraView(m_camera, 45.0f, aspectRatio, 0.1f, 1000.0f);

		H3DRes cameraPipeRes = h3dGetNodeParamI(m_camera, H3DCamera::PipeResI);
		h3dResizePipelineBuffers(cameraPipeRes, deviceWidth, deviceHeight);
	} // end if
} // end updateFBO

void Horde3DItem::setSize(const QSize &size)
{
    if (size == m_size)
        return;

    m_size = size;
    m_dirtyFBO = true;

    m_node->markDirty(QSGNode::DirtyGeometry);
} // end setSize

void Horde3DItem::setAAEnabled(bool enable)
{
    if (m_AAEnabled == enable)
        return;

    m_AAEnabled = enable;
    m_dirtyFBO = true;

    // markDirty(DirtyMaterial) gets called every frame, so we don't actually need to call it here.
    //m_node->markDirty(QSGNode::DirtyMaterial);
} // end setAAEnabled

void Horde3DItem::init()
{
    const QGLContext *ctx = QGLContext::currentContext();
    QGLFormat format = ctx->format();
    m_samples = format.sampleBuffers() ? format.samples() : 0;

	if(!h3dInit())
	{
		qCritical() << "h3dInit failed";
	}

    pipeline = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
    H3DRes knight = h3dAddResource(H3DResTypes::SceneGraph, "models/knight/knight.scene.xml", 0);

    h3dutLoadResourcesFromDisk("Content");

    H3DNode node = h3dAddNodes(H3DRootNode, knight);
    h3dSetNodeTransform(node,
			0, 0, 0,
			0, 0, 0,
			1, 1, 1
			);

	m_camera = h3dAddCameraNode(H3DRootNode, "cam", pipeline);
	m_cameraObject = new CameraNodeObject(m_camera);
    m_cameraQObject = static_cast<QObject *>(m_cameraObject);

    m_initialized = true;
} // end init
