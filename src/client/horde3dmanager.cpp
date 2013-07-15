#include <Horde3DUtils.h>

#include "config.h"
#include "entity.h"
#include "horde3dmanager.h"


Horde3DManager::Horde3DManager(QObject* parent) :
		QObject(parent),
		_initialized(false),
		_avatar(NULL),
		_scene(NULL),
		_loadTimer(0),
		_logger(PLogManager::getLogger("horde3d")),
		_settings(PSettingsManager::instance())
{
	connect(this, SIGNAL(resourceLoaded(H3DRes)), this, SLOT(onResourceLoaded(H3DRes)), Qt::DirectConnection);
} // end Horde3DManager

Horde3DManager::~Horde3DManager()
{
} // end ~Horde3DManager


Horde3DManager& Horde3DManager::instance()
{
	static Horde3DManager _instance;
	return _instance;
} // end instance


// Getters //

Entity* Horde3DManager::root() const
{
	return Entity::getEntity(H3DRootNode);
} // end root

Entity* Horde3DManager::avatar() const
{
	return _avatar;
} // end avatar

Entity* Horde3DManager::scene() const
{
	return _scene;
} // end scene

QMultiHash<QString, Entity*> Horde3DManager::skyboxes() const
{
	return _skyboxes;
} // end contentDirs

QString Horde3DManager::contentDirs() const
{
	return _settings.get(CFG_CONTENT_DIRS, DEFAULT_CONTENT_DIRS).toString();
} // end contentDirs

QString Horde3DManager::defaultPipeline() const
{
	return _settings.get(CFG_DEFAULT_PIPELINE, DEFAULT_DEFAULT_PIPELINE).toString();
} // end defaultPipeline

int Horde3DManager::anisotropy() const
{
	return _settings.get(CFG_ANISOTROPY, DEFAULT_ANISOTROPY).toInt();
} // end anisotropy

int Horde3DManager::antiAliasingSamples() const
{
	return _settings.get(CFG_AA_SAMPLES, DEFAULT_AA_SAMPLES).toInt();
} // end antiAliasingSamples


// Setters //

void Horde3DManager::setAvatar(Entity* avatar)
{
	_avatar = avatar;
	emit avatarChanged(_avatar);
} // end setAvatar

void Horde3DManager::setScene(Entity* scene)
{
	_scene = scene;
	emit sceneChanged(_scene);
} // end setScene

void Horde3DManager::setAnisotropy(int textureAnisotropy)
{
	if(!h3dSetOption(H3DOptions::MaxAnisotropy, textureAnisotropy))
	{
		_logger.error(QString("Couldn't set texture anisotropy to %1!").arg(textureAnisotropy));
	}
	else
	{
		_settings.set(CFG_ANISOTROPY, textureAnisotropy);
		emit anisotropyChanged(textureAnisotropy);
	} // end if
} // end setAnisotropy

void Horde3DManager::setAntiAliasingSamples(int samples)
{
	if(!h3dSetOption(H3DOptions::SampleCount, samples))
	{
		_logger.error(QString("Couldn't set antialiasing samples to %1!").arg(samples));
	}
	else
	{
		_settings.set(CFG_AA_SAMPLES, samples);
		emit antiAliasingSamplesChanged(samples);
	} // end if
} // end setAnisotropy


bool Horde3DManager::loadResource(H3DResTypes::List type, QString path, int flags, H3DRes* resRef)
{
	H3DRes res = h3dAddResource(type, path.toUtf8().constData(), flags);
	if(res == 0)
	{
		_logger.error(QString("Failed to create resource handle for %1!").arg(path));
		return false;
	} // end if

	if(!h3dIsResLoaded(res))
	{
		_queuedResources.append(res);

		if(_loadTimer == 0)
		{
			_loadTimer = startTimer(0);
		} // end if
	} // end if

	if(resRef != NULL)
	{
		*resRef = res;
	} // end if
	return true;
} // end loadResource


bool Horde3DManager::isSceneLoaded(QString sceneID)
{
	return _loadedScenes.contains(sceneID);
} // end isSceneLoaded

Entity* Horde3DManager::getScene(QString sceneID)
{
	return _loadedScenes[sceneID];
} // end getScene

bool Horde3DManager::loadScene(QString sceneID, int flags)
{
	H3DRes res;

	if(!loadResource(H3DResTypes::SceneGraph, sceneID.toUtf8().constData(), flags, &res))
	{
		return false;
	} // end if

	_queuedScenes[res] = sceneID;
	return true;
} // end loadScene

bool Horde3DManager::switchScene(QString sceneID, bool keep)
{
	if(!isSceneLoaded(sceneID))
	{
		_logger.warn(QString("Can't switch to scene \"%1\"! Scene not loaded.").arg(sceneID));
		return false;
	} // end if

	if(_scene == _loadedScenes[sceneID])
	{
		_logger.warn("Ignoring switch to already-current scene.");
		return true;
	} // end if

	Entity* oldScene = _scene;
	_scene = _loadedScenes[sceneID];

	if(!keep && oldScene)
	{
		if(oldScene->contains(_avatar))
		{
			_avatar->setParent(root());
		} // end if

		oldScene->remove();
	} // end if

	return true;
} // end switchScene

void Horde3DManager::unloadScene(QString sceneID)
{
	_logger.info(QString("Unloading scene \"%1\"...").arg(sceneID));
	if(!isSceneLoaded(sceneID))
	{
		_logger.warn(QString("Trying to unload scene \"%1\" which is not loaded!").arg(sceneID));
		return;
	} // end if

	Entity* scene = _loadedScenes.take(sceneID);

	if(scene->contains(_avatar))
	{
		_avatar->setParent(root());
	} // end if

	if(scene == _scene)
	{
		_logger.notify(QString("Unload current scene \"%1\".").arg(sceneID));
	} // end if

	scene->remove();
} // end unloadScene

void Horde3DManager::renderScene(Entity* camera, QString sceneID)
{
	Entity* scene = _scene;

	if(!sceneID.isNull())
	{
		if(!isSceneLoaded(sceneID))
		{
			_logger.error(QString("Trying to render scene \"%1\" which is not loaded!").arg(sceneID));
			return;
		} // end if

		scene = _loadedScenes[sceneID];
	} // end if

	if(!scene)
	{
		_logger.error("Trying to render NULL scene!");
		return;
	} // end if

	Entity::Flags oldFlags = scene->flags();
	//scene->setFlagsRecursive(Entity::Flags(oldFlags & !Entity::EF_INACTIVE));
	scene->setFlags(Entity::Flags(oldFlags & !Entity::EF_INACTIVE));

	h3dRender(camera->node());

	scene->setFlags(oldFlags);

	h3dFinalizeFrame();
} // end unloadScene


void Horde3DManager::addSkybox(QString sceneID, Entity* skybox)
{
	_skyboxes.insertMulti(sceneID, skybox);
} // end addSkybox


void Horde3DManager::init()
{
	_logger.debug("Horde3DManager::init()");

	if(!h3dInit())
	{
		_logger.fatal("h3dInit failed");
	} // end if

	setAnisotropy(anisotropy());

	_avatar = root()->loadModel("models/ares/ares.scene.xml");
	loadScene("scenes/asteroids/asteroids.scene.xml");

	printHordeMessages();
	_logger.debug("--------- Initialization Finished ---------");

	_initialized = true;
	emit initFinished();
} // end init

void Horde3DManager::update()
{
	Entity::runScheduled();

	printHordeMessages();
} // end update

void Horde3DManager::printHordeMessages()
{
	int msgLevel;
	float msgTime;
	const char* message;

	while((message = h3dGetMessage(&msgLevel, &msgTime)) && strlen(message) > 0)
	{
		_logger.debug(QString("%1 message from Horde3D at %2: %3").arg(msgLevel).arg(msgTime).arg(message));
	} // end while
} // end printHordeMessages

bool Horde3DManager::loadNewResources()
{
	_logger.debug(QString("Loading new resources from: %1").arg(contentDirs()));
	if(!h3dutLoadResourcesFromDisk(contentDirs().toUtf8().constData()))
	{
		_logger.error("Failed to load new resources:");
		printHordeMessages();
		return false;
	} // end if
	return true;
} // end loadNewResources

H3DRes Horde3DManager::loadPipeline(QString pipelineName)
{
	QString isDefault;
	if(pipelineName.isNull())
	{
		pipelineName = defaultPipeline();
		isDefault = " default";
	} // end if

	H3DRes pipeline = h3dAddResource(H3DResTypes::Pipeline, pipelineName.toUtf8().constData(), 0);
	if(!loadNewResources())
	{
		_logger.error(QString("Couldn't load%1 rendering pipeline %2!").arg(isDefault, pipelineName));
		return 0;
	} // end if

	return pipeline;
} // end loadPipeline

void Horde3DManager::timerEvent(QTimerEvent* event)
{
	if(event->timerId() == _loadTimer)
	{
		_logger.debug(QString("Loading %1 resources from: %2").arg(_queuedResources.length()).arg(contentDirs()));
		if(!loadNewResources())
		{
			_logger.error("Failed to load queued resources!");
			printHordeMessages();
		} // end if

		for(int i = 0; i < _queuedResources.size(); i++)
		{
			H3DRes res = _queuedResources.at(i);
			if(h3dIsResLoaded(res))
			{
				_queuedResources.removeAt(i);
				i--;
				emit resourceLoaded(res);
			}
			else
			{
				_logger.debug(QString("Resource \"%1\" (type %2) was not loaded.")
						.arg(h3dGetResName(res))
						.arg(h3dGetResType(res))
						);
			} // end if
		} // end for

		killTimer(_loadTimer);
		_loadTimer = 0;
	} // end if
} // end timerEvent

void Horde3DManager::onResourceLoaded(H3DRes resource)
{
	QString sceneID = _queuedScenes.take(resource);
	if(!sceneID.isNull())
	{
		H3DNode sceneNode = h3dAddNodes(root()->node(), resource);

		if(sceneNode == 0)
		{
			_logger.error(QString("Failed to create node for scene \"%1\"!").arg(sceneID));
		}
		else
		{
			_logger.debug(QString("Loaded scene \"%1\".").arg(sceneID));

			_loadedScenes[sceneID] = Entity::getEntity(sceneNode);
			emit sceneLoaded(sceneID, _loadedScenes[sceneID]);

			if(!_scene)
			{
				_logger.info(QString("No current scene yet; setting loaded scene \"%1\" as current.").arg(sceneID));
				_scene = _loadedScenes[sceneID];
			} // end if
		} // end if
	} // end if
} // end onResourceLoaded
