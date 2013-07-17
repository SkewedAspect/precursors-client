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
		_initialized(false),
		_dirtyView(false),
		_shipRot(0.0f),
		_qtContext(NULL),
		_h3dContext(NULL),
		_camDolly(NULL),
		_camera(NULL),
		_skybox(NULL),
		_logger(PLogManager::getLogger("horde3dwindow")),
		_settings(PSettingsManager::instance()),
		_mgr(Horde3DManager::instance())
{
	lastFrameStart.start();

	connect(this, SIGNAL(beforeRendering()), this, SLOT(onBeforeRendering()), Qt::DirectConnection);

	connect(&_mgr, SIGNAL(initFinished()), this, SLOT(onInitFinished()), Qt::QueuedConnection);
	connect(&_mgr, SIGNAL(sceneChanged(const Entity*, QString)),
			this, SLOT(onSceneChanged(const Entity*, QString)),
			Qt::QueuedConnection
			);
} // end Horde3DWindow

Horde3DWindow::~Horde3DWindow()
{
	if(_h3dContext)
	{
		_h3dContext->deleteLater();
		_h3dContext = NULL;
	} // end if
} // end ~Horde3DWindow


Entity* Horde3DWindow::camera() const
{
	return _camera;
} // end camera

Entity* Horde3DWindow::camDolly() const
{
	return _camDolly;
} // end camDolly

float Horde3DWindow::fps() const
{
	return lastFPS;
} // end fps

float Horde3DWindow::maxViewDistance() const
{
	return _settings.get(CFG_VIEW_DIST, DEFAULT_VIEW_DIST).toFloat();
} // end maxViewDistance


void Horde3DWindow::setMaxViewDistance(float maxViewDist)
{
	h3dSetNodeParamF(camera()->node(), H3DCamera::FarPlaneF, 0, maxViewDist);

	_settings.set(CFG_VIEW_DIST, maxViewDist);
	emit maxViewDistanceChanged(maxViewDist);
} // end setMaxViewDistance


void Horde3DWindow::renderHorde()
{
	_mgr.update();

	if(_camera)
	{
		if(!USE_SEPARATE_CONTEXT || (_qtContext && _h3dContext))
		{
			restoreH3DState();

			_mgr.renderScene(_camera);

			saveH3DState();
		} // end if
	} // end if
} // end renderHorde

void Horde3DWindow::onSceneChanged(const Entity* scene, QString sceneID)
{
	_logger.debug(QString("Handling onSceneChanged(%1, \"%2\")...").arg(scene->toString()).arg(sceneID));

	if(_skybox)
	{
		_logger.debug(QString("Reparenting old skybox %1 back to its scene...").arg(_skybox->toString()));

		Entity* skyboxParent = _mgr.root();
		if(_mgr.skyboxes().contains(_sceneID, _skybox))
		{
			skyboxParent = _mgr.getScene(_sceneID);
		} // end if

		_skybox->setParent(skyboxParent);
		_skybox = NULL;
	} // end if

	_sceneID = sceneID;

	if(_mgr.skyboxes().contains(sceneID))
	{
		_logger.debug(QString("Setting up skybox for scene \"%1\"...").arg(sceneID));
		_skybox = _mgr.skyboxes().value(sceneID);
		_skybox->setParent(_camera);
	} // end if
} // end onSceneChanged

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
		h3dSetNodeParamI(_camera->node(), H3DCamera::ViewportXI, 0);
		h3dSetNodeParamI(_camera->node(), H3DCamera::ViewportYI, 0);
		h3dSetNodeParamI(_camera->node(), H3DCamera::ViewportWidthI, deviceWidth);
		h3dSetNodeParamI(_camera->node(), H3DCamera::ViewportHeightI, deviceHeight);

		// Set virtual camera parameters
		float aspectRatio = static_cast<float>(deviceWidth) / deviceHeight;
		h3dSetupCameraView(_camera->node(), 45.0f, aspectRatio, 0.01f, maxViewDistance());
		//_camDolly->scheduleOnce();
		_camera->scheduleOnce();

		H3DRes cameraPipeRes = h3dGetNodeParamI(_camera->node(), H3DCamera::PipeResI);
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

	restoreQtState();
} // end onBeforeRendering

void Horde3DWindow::onInitFinished()
{
	startTimer(16);
} // end onInitFinished

void Horde3DWindow::init()
{
	qDebug() << "Horde3DWindow::init()";

	_mgr.init();

	_qtContext = openglContext();
	_mgr.setAntiAliasingSamples(_qtContext->format().samples());

	_camDolly = _mgr.root()->newGroup("camera dolly");
	_camera = _camDolly->newCamera("cam");
	_camera->setPos(0, 20, 100);
	emit cameraChanged(_camera);

	setClearBeforeRendering(false);

	_mgr.printHordeMessages();
	qDebug() << "--------- Initialization Finished ---------";

	_size = size() * screen()->devicePixelRatio();
	_dirtyView = true;

	_initialized = true;
} // end init
