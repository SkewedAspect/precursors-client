#include "../config.h"
#include "horde3dwindow.h"
#include "cameranodeobject.h"

#include <Horde3DUtils.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QScreen>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGSimpleTextureNode>

#ifdef __APPLE__
	// Because Apple loves being a special snowflake?
#	include <OpenGL/gl.h>
#else
#	include <GL/gl.h>
#endif


static const bool USE_SEPARATE_CONTEXT = false;


Horde3DWindow::Horde3DWindow(QWindow *parent) :
		QQuickWindow(parent),
		m_samples(0),
		m_initialized(false),
		m_dirtyView(false),
		m_animTime(0.0f),
		m_qtContext(NULL),
		m_h3dContext(NULL)
{
	connect(this, SIGNAL(beforeRendering()), this, SLOT(onBeforeRendering()), Qt::DirectConnection);
	connect(this, SIGNAL(initFinished()), this, SLOT(onInitFinished()), Qt::QueuedConnection);
} // end Horde3DWindow

Horde3DWindow::~Horde3DWindow()
{
	if(m_h3dContext)
	{
		m_h3dContext->deleteLater();
		m_h3dContext = NULL;
	} // end if
} // end ~Horde3DWindow

void Horde3DWindow::renderHorde()
{
	m_animTime += 0.4f;

	// Do animation blending
	h3dSetModelAnimParams(m_knight, 0, m_animTime, 0.5f);
	h3dSetModelAnimParams(m_knight, 1, m_animTime, 0.5f);
	h3dUpdateModel(m_knight, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);

	if(m_camera)
	{
		if(!USE_SEPARATE_CONTEXT || (m_qtContext && m_h3dContext))
		{
			restoreH3DState();

			h3dRender(m_camera);
			h3dFinalizeFrame();

			saveH3DState();
		} // end if
	} // end if
} // end renderHorde

void Horde3DWindow::resizeEvent(QResizeEvent* event)
{
	qDebug() << "Horde3DWindow::resizeEvent(" << event->oldSize() << "->" << event->size() << "); devicePixelRatio:" << screen()->devicePixelRatio();

	if(event->size() != event->oldSize())
	{
		m_size = event->size() * screen()->devicePixelRatio();
		m_dirtyView = true;
	} // end if

	QQuickWindow::resizeEvent(event);
} // end geometryChanged

void Horde3DWindow::printHordeMessages()
{
	int msgLevel;
	float msgTime;
	const char* message;

	while((message = h3dGetMessage(&msgLevel, &msgTime)) && strlen(message) > 0)
	{
		qDebug() << msgLevel << "message from Horde3D at" << msgTime << ":" << message;
	} // end while
} // end printHordeMessages

void Horde3DWindow::restoreH3DState()
{
	if(USE_SEPARATE_CONTEXT)
	{
		m_qtContext->doneCurrent();
		m_h3dContext->makeCurrent(this);
	}
	else
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
	} // end if

	QOpenGLFunctions glFunctions(QOpenGLContext::currentContext());
	glFunctions.glUseProgram(0);

	if(renderTarget())
	{
		renderTarget()->bind();
	} // end if
} // end restoreH3DState

void Horde3DWindow::saveH3DState()
{
	if(renderTarget())
	{
		renderTarget()->release();
	} // end if

	if(USE_SEPARATE_CONTEXT)
	{
		m_h3dContext->doneCurrent();
		m_qtContext->makeCurrent(this);
	}
	else
	{
		glPopAttrib();
	} // end if
} // end saveH3DState

void Horde3DWindow::updateView()
{
	if(m_initialized)
	{
		qDebug() << "Horde3DWindow::updateView()";

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
				23, -166, 0,
				1, 1, 1
				);

		H3DRes cameraPipeRes = h3dGetNodeParamI(m_camera, H3DCamera::PipeResI);
		h3dResizePipelineBuffers(cameraPipeRes, deviceWidth, deviceHeight);

		m_dirtyView = false;
	} // end if
} // end updateView

void Horde3DWindow::timerEvent(QTimerEvent *)
{
	update();
} // end timerEvent

void Horde3DWindow::onBeforeRendering()
{
	if(!m_initialized)
	{
		init();
	} // end if

	if(m_dirtyView)
	{
		updateView();
	} // end if

	if(USE_SEPARATE_CONTEXT)
	{
		if(m_h3dContext && (m_h3dContext->format() != m_qtContext->format()))
		{
			m_h3dContext->deleteLater();
			m_h3dContext = NULL;
		} // end if

		if(!m_h3dContext)
		{
			qDebug() << "Creating new OpenGL context.";
			// Create a new shared OpenGL context to be used exclusively by Horde3D
			m_h3dContext = new QOpenGLContext();
			m_h3dContext->setFormat(requestedFormat());
			m_h3dContext->setShareContext(m_qtContext);
			m_h3dContext->create();
		} // end if
	} // end if

	renderHorde();

	printHordeMessages();
} // end onBeforeRendering

void Horde3DWindow::onInitFinished()
{
	startTimer(16);
} // end onInitFinished

void Horde3DWindow::init()
{
	qDebug() << "Horde3DWindow::init()";

	m_qtContext = openglContext();
	m_samples = m_qtContext->format().samples();

	if(!h3dInit())
	{
		qCritical() << "h3dInit failed";
	} // end if

	int textureAnisotropy = 8;
	if(!h3dSetOption(H3DOptions::MaxAnisotropy, textureAnisotropy))
	{
		qDebug() << "Couldn't set texture anisotropy to" << textureAnisotropy << "!";
	} // end if

	if(!h3dSetOption(H3DOptions::SampleCount, m_samples))
	{
		qDebug() << "Couldn't set antialiasing samples to" << m_samples << "!";
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

	setClearBeforeRendering(false);

	printHordeMessages();
	qDebug() << "--------- Initialization Finished ---------";

	m_size = size() * screen()->devicePixelRatio();
	m_dirtyView = true;

	m_initialized = true;
	emit initFinished();
} // end init
