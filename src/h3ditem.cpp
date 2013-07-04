#include "../config.h"
#include "h3ditem.h"
#include "cameranodeobject.h"

#include <Horde3DUtils.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGSimpleTextureNode>

#define USE_BEFORE_RENDER 1


Horde3DItem::Horde3DItem(QQuickItem *parent) :
		QQuickItem(parent),
		m_timerID(0),
		m_cameraQObject(NULL),
		m_node(NULL),
		m_fbo(NULL),
		m_texture(NULL),
		m_qtContext(NULL),
		m_h3dContext(NULL),
		m_samples(0),
		m_AAEnabled(false),
		m_initialized(false),
		m_dirtyFBO(false),
		m_animTime(0.0f)
{
    setFlag(ItemHasContents);
    setSmooth(false);

	connect(this, SIGNAL(initFinished()), this, SLOT(onInitFinished()), Qt::QueuedConnection);
} // end Horde3DItem

Horde3DItem::~Horde3DItem()
{
	//TODO: Cleanup!
} // end ~Horde3DItem

QSGNode* Horde3DItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data)
{
	Q_UNUSED(data);

	if(!m_initialized)
	{
		init();
	} // end if

	if(m_dirtyFBO)
	{
		updateFBO();
	} // end if

    if(width() <= 0 || height() <= 0 || !m_texture)
	{
        delete oldNode;
        return 0;
    } // end if

    m_node = static_cast<QSGSimpleTextureNode *>(oldNode);
    if(!m_node)
    {
        m_node = new QSGSimpleTextureNode();
        m_node->setFiltering(QSGTexture::Nearest);
    } // end if

    m_node->setRect(boundingRect());
	m_node->setTexture(m_texture);

    setAAEnabled(smooth());

#	ifndef USE_BEFORE_RENDER
		renderHorde();
#	endif // USE_BEFORE_RENDER

    return m_node;
} // end updatePaintNode

void Horde3DItem::renderHorde()
{
	if(m_node)
	{
		m_animTime += 0.4f;

		// Do animation blending
		h3dSetModelAnimParams(m_knight, 0, m_animTime, 0.5f);
		h3dSetModelAnimParams(m_knight, 1, m_animTime, 0.5f);
		h3dUpdateModel(m_knight, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);

		if(m_qtContext && m_h3dContext && m_fbo && m_camera)
		{
			if(restoreH3DState())
			{
				h3dRender(m_camera);
				h3dFinalizeFrame();
			} // end if

			saveH3DState();
		} // end if

		m_node->markDirty(QSGNode::DirtyMaterial);
	} // end if
} // end renderHorde

void Horde3DItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	qDebug() << "Horde3DItem::geometryChanged(" << newGeometry << "," << oldGeometry << ")";

	if(newGeometry.size() != oldGeometry.size())
	{
		m_size = QSize((int) newGeometry.width(), (int) newGeometry.height());
		m_dirtyFBO = true;

		if(m_node)
		{
			m_node->markDirty(QSGNode::DirtyGeometry);
		} // end if
	} // end if
} // end geometryChanged

void Horde3DItem::printHordeMessages()
{
	int msgLevel;
	float msgTime;
	const char* message;

	while((message = h3dGetMessage(&msgLevel, &msgTime)) && strlen(message) > 0)
	{
		qDebug() << msgLevel << "message from Horde3D at" << msgTime << ":" << message;
	} // end while
} // end printHordeMessages

bool Horde3DItem::restoreH3DState()
{
    m_qtContext->doneCurrent();
    m_h3dContext->makeCurrent(window());

	QOpenGLFunctions glFunctions(m_h3dContext);
    glFunctions.glUseProgram(0);

	if(!m_fbo->bind())
	{
		qCritical() << "Error binding FBO!";
		return false;
	}
	else
	{
		return true;
	} // end if
} // end restoreH3DState

void Horde3DItem::saveH3DState()
{
	if(!m_fbo->release())
	{
		qCritical() << "Error releasing FBO!";
	} // end if

    m_h3dContext->doneCurrent();
    m_qtContext->makeCurrent(window());
} // end saveH3DState

void Horde3DItem::updateFBO()
{
	qDebug() << "Horde3DItem::updateFBO()";

	if(m_fbo)
	{
		delete m_fbo;
	} // end if

	m_qtContext = QOpenGLContext::currentContext();

	if(m_h3dContext && (m_h3dContext->format() != m_qtContext->format()))
	{
		m_h3dContext->deleteLater();
		m_h3dContext = NULL;
	} // end if

	if(!m_h3dContext)
	{
		// Create a new shared OpenGL context to be used exclusively by Horde3D
		m_h3dContext = new QOpenGLContext();
		m_h3dContext->setFormat(window()->requestedFormat());
		m_h3dContext->setShareContext(m_qtContext);
		m_h3dContext->create();
	} // end if

    m_h3dContext->makeCurrent(window());
	m_fbo = new QOpenGLFramebufferObject(m_size, QOpenGLFramebufferObject::CombinedDepthStencil);
    m_h3dContext->doneCurrent();

	int deviceWidth = m_size.width();
	int deviceHeight = m_size.height();

	// Resize viewport
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportXI, 0);
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportYI, 0);
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportWidthI, deviceWidth);
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportHeightI, deviceHeight);

	// Set virtual camera parameters
	float aspectRatio = static_cast<float>(deviceWidth) / deviceHeight;
	h3dSetupCameraView(m_camera, 45.0f, aspectRatio, 0.1f, 1000.0f);
    h3dSetNodeTransform(m_camera,
			0, 40, -40,
			//-40, 40, -70,
			23, -166, 180,
			1, 1, 1
			);

	H3DRes cameraPipeRes = h3dGetNodeParamI(m_camera, H3DCamera::PipeResI);
	h3dResizePipelineBuffers(cameraPipeRes, deviceWidth, deviceHeight);

	delete m_texture;
	m_texture = window()->createTextureFromId(m_fbo->texture(), m_fbo->size());

	m_texture->setFiltering(QSGTexture::Nearest);
	m_texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
	m_texture->setVerticalWrapMode(QSGTexture::ClampToEdge);

    m_dirtyFBO = false;

	// Appease the angry QML gods.
	restoreH3DState();
	saveH3DState();
} // end updateFBO

void Horde3DItem::setAAEnabled(bool enable)
{
    if(m_AAEnabled != enable)
	{
		qDebug() << "Horde3DItem::setAAEnabled(" << enable << ")";

		m_AAEnabled = enable;
		m_dirtyFBO = true;
	} // end if
} // end setAAEnabled

void Horde3DItem::timerEvent(QTimerEvent *)
{
	printHordeMessages();

	update();
} // end timerEvent

void Horde3DItem::onBeforeRendering()
{
	renderHorde();

	printHordeMessages();
} // end onBeforeRendering

void Horde3DItem::onInitFinished()
{
	startTimer(16);

#	ifdef USE_BEFORE_RENDER
		connect(window(), SIGNAL(beforeRendering()), this, SLOT(onBeforeRendering()), Qt::DirectConnection);
#	endif // USE_BEFORE_RENDER
} // end onInitFinished

void Horde3DItem::init()
{
	qDebug() << "Horde3DItem::init()";

    const QOpenGLContext *ctx = window()->openglContext();
    m_samples = ctx->format().samples();

	if(!h3dInit())
	{
		qCritical() << "h3dInit failed";
	} // end if

	int textureAnisotropy = 8;
	if(!h3dSetOption(H3DOptions::MaxAnisotropy, textureAnisotropy))
	{
		qDebug() << "Couldn't set texture anisotropy to" << textureAnisotropy << "!";
	} // end if

	int aaSamples = m_AAEnabled ? m_samples : 0;
	if(!h3dSetOption(H3DOptions::SampleCount, aaSamples))
	{
		qDebug() << "Couldn't set antialiasing samples to" << aaSamples << "!";
	} // end if

    H3DRes pipeline = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
    H3DRes knight = h3dAddResource(H3DResTypes::SceneGraph, "models/knight/knight.scene.xml", 0);
	H3DRes knightAnim1Res = h3dAddResource( H3DResTypes::Animation, "animations/knight_order.anim", 0 );
	H3DRes knightAnim2Res = h3dAddResource( H3DResTypes::Animation, "animations/knight_attack.anim", 0 );

    h3dutLoadResourcesFromDisk("Content");

    m_knight = h3dAddNodes(H3DRootNode, knight);
    h3dSetNodeTransform(m_knight,
			0, 0, 0,
			0, 0, 0,
			1, 1, 1
			);
	h3dSetupModelAnimStage(m_knight, 0, knightAnim1Res, 0, "", false);
	h3dSetupModelAnimStage(m_knight, 1, knightAnim2Res, 0, "", false);

	m_camera = h3dAddCameraNode(H3DRootNode, "cam", pipeline);
	h3dSetNodeParamF(m_camera, H3DCamera::FarPlaneF, 0, 100000);

	m_cameraObject = new CameraNodeObject(m_camera);
    m_cameraQObject = static_cast<QObject *>(m_cameraObject);

    m_initialized = true;

	printHordeMessages();
	qDebug() << "--------- Initialization Finished ---------";

	emit initFinished();
} // end init
