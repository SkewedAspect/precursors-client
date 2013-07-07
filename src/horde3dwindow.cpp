#include "config.h"
#include "horde3dwindow.h"
#include "entity.h"

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


Horde3DWindow::Horde3DWindow(QWindow* parent) :
		QQuickWindow(parent),
		_samples(0),
		_initialized(false),
		_dirtyView(false),
		_shipRot(0.0f),
		_qtContext(NULL),
		_h3dContext(NULL),
		_avatarEnt(NULL),
		_sceneEnt(NULL),
		_camDollyEnt(NULL),
		_cameraEnt(NULL)
{
	lastFrameStart.start();

	connect(this, SIGNAL(beforeRendering()), this, SLOT(onBeforeRendering()), Qt::DirectConnection);
	connect(this, SIGNAL(initFinished()), this, SLOT(onInitFinished()), Qt::QueuedConnection);
} // end Horde3DWindow

Horde3DWindow::~Horde3DWindow()
{
	if(_h3dContext)
	{
		_h3dContext->deleteLater();
		_h3dContext = NULL;
	} // end if
} // end ~Horde3DWindow

void Horde3DWindow::renderHorde()
{
	// Do animation blending
	//h3dSetModelAnimParams(_knight, 0, _animTime, 0.5f);
	//h3dSetModelAnimParams(_knight, 1, _animTime, 0.5f);
	//h3dUpdateModel(_knight, H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry);

	/*
	_shipRot += lastFrameTime / 10.0;
	while(_shipRot > 360)
	{
		_shipRot -= 360;
	} // end while

	h3dSetNodeTransform(_avatar,
			0, 0, 0,
			0, _shipRot, 0,
			1, 1, 1
			);
	_avatarEnt->setHeading(_shipRot);
	*/

	Entity::runScheduled();

	if(_camera)
	{
		if(!USE_SEPARATE_CONTEXT || (_qtContext && _h3dContext))
		{
			restoreH3DState();

			h3dRender(_camera);
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
		_size = event->size() * screen()->devicePixelRatio();
		_dirtyView = true;
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

void Horde3DWindow::saveQtState()
{
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
} // end saveQtState

void Horde3DWindow::restoreQtState()
{
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
	glPopClientAttrib();
} // end restoreQtState

void Horde3DWindow::restoreH3DState()
{
	if(USE_SEPARATE_CONTEXT)
	{
		_qtContext->doneCurrent();
		_h3dContext->makeCurrent(this);
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
		_h3dContext->doneCurrent();
		_qtContext->makeCurrent(this);
	} // end if
} // end saveH3DState

void Horde3DWindow::updateView()
{
	if(_initialized)
	{
		qDebug() << "Horde3DWindow::updateView()";

		int deviceWidth = _size.width();
		int deviceHeight = _size.height();

		// Resize viewport
		h3dSetNodeParamI(_camera, H3DCamera::ViewportXI, 0);
		h3dSetNodeParamI(_camera, H3DCamera::ViewportYI, 0);
		h3dSetNodeParamI(_camera, H3DCamera::ViewportWidthI, deviceWidth);
		h3dSetNodeParamI(_camera, H3DCamera::ViewportHeightI, deviceHeight);

		// Set virtual camera parameters
		float aspectRatio = static_cast<float>(deviceWidth) / deviceHeight;
		h3dSetupCameraView(_camera, 45.0f, aspectRatio, 0.1f, 1000.0f);
		_camDollyEnt->apply();
		_cameraEnt->apply();

		H3DRes cameraPipeRes = h3dGetNodeParamI(_camera, H3DCamera::PipeResI);
		h3dResizePipelineBuffers(cameraPipeRes, deviceWidth, deviceHeight);

		_dirtyView = false;
	} // end if
} // end updateView

void Horde3DWindow::timerEvent(QTimerEvent* event)
{
	Q_UNUSED(event);

	update();
} // end timerEvent

void Horde3DWindow::onBeforeRendering()
{
	float frameTime = lastFrameStart.elapsed();
	lastFrameStart.restart();

	if(frameTime > 0.0000000000000000001f)
	{
		lastFrameTime = frameTime;
		lastFPS = 1000.0 / lastFrameTime;
		emit fpsChanged(lastFPS);
	} // end if

	saveQtState();

	if(!_initialized)
	{
		init();
	} // end if

	if(_dirtyView)
	{
		updateView();
	} // end if

	if(USE_SEPARATE_CONTEXT)
	{
		if(_h3dContext && (_h3dContext->format() != _qtContext->format()))
		{
			_h3dContext->deleteLater();
			_h3dContext = NULL;
		} // end if

		if(!_h3dContext)
		{
			qDebug() << "Creating new OpenGL context.";
			// Create a new shared OpenGL context to be used exclusively by Horde3D
			_h3dContext = new QOpenGLContext();
			_h3dContext->setFormat(requestedFormat());
			_h3dContext->setShareContext(_qtContext);
			_h3dContext->create();
		} // end if
	} // end if

	renderHorde();

	printHordeMessages();

	restoreQtState();
} // end onBeforeRendering

void Horde3DWindow::onInitFinished()
{
	startTimer(16);
} // end onInitFinished

void Horde3DWindow::init()
{
	qDebug() << "Horde3DWindow::init()";

	_qtContext = openglContext();
	_samples = _qtContext->format().samples();

	if(!h3dInit())
	{
		qCritical() << "h3dInit failed";
	} // end if

	int textureAnisotropy = 8;
	if(!h3dSetOption(H3DOptions::MaxAnisotropy, textureAnisotropy))
	{
		qDebug() << "Couldn't set texture anisotropy to" << textureAnisotropy << "!";
	} // end if

	if(!h3dSetOption(H3DOptions::SampleCount, _samples))
	{
		qDebug() << "Couldn't set antialiasing samples to" << _samples << "!";
	} // end if

	H3DRes pipeline = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
	H3DRes avatar = h3dAddResource(H3DResTypes::SceneGraph, "models/ares/ares.scene.xml", 0);
	H3DRes scene = h3dAddResource(H3DResTypes::SceneGraph, "scenes/asteroids/asteroids.scene.xml", 0);

	h3dutLoadResourcesFromDisk("content");

	_avatar = h3dAddNodes(H3DRootNode, avatar);
	h3dSetNodeTransform(_avatar,
			0, 0, 0,
			0, 0, 0,
			1, 1, 1
			);
	_avatarEnt = Entity::getEntity(_avatar);

	_scene = h3dAddNodes(H3DRootNode, scene);
	_sceneEnt = Entity::getEntity(_scene);

	_camDolly = h3dAddGroupNode(H3DRootNode, "camera dolly");
	_camDollyEnt = Entity::getEntity(_camDolly);

	_camera = h3dAddCameraNode(_camDolly, "cam", pipeline);
	h3dSetNodeParamF(_camera, H3DCamera::FarPlaneF, 0, 100000);
	h3dSetNodeTransform(_camera,
			0, 0, 100,
			0, 0, 0,
			1, 1, 1
			);
	_cameraEnt = Entity::getEntity(_camera);

	setClearBeforeRendering(false);

	printHordeMessages();
	qDebug() << "--------- Initialization Finished ---------";

	_size = size() * screen()->devicePixelRatio();
	_dirtyView = true;

	_initialized = true;
	emit initFinished();
} // end init
