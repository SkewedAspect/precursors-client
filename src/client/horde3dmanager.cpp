#include <Horde3DUtils.h>

#include "config.h"
#include "entity.h"
#include "horde3dmanager.h"


Horde3DManager::Horde3DManager(QObject* parent) :
        QObject(parent),
        _initialized(false),
        _avatar(NULL),
        _scene(NULL),
        _logger(PLogManager::getLogger("horde3d")),
        _settings(PSettingsManager::instance())
{
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
        _settings.set(CFG_ANISOTROPY, textureAnisotropy);
    } // end if
} // end setAnisotropy

void Horde3DManager::setAntiAliasingSamples(int samples)
{
    if(!h3dSetOption(H3DOptions::SampleCount, samples))
    {
        _logger.error(QString("Couldn't set antialiasing samples to %1!").arg(samples));
        _settings.set(CFG_AA_SAMPLES, samples);
    } // end if
} // end setAnisotropy


void Horde3DManager::init()
{
    _logger.debug("Horde3DManager::init()");

    if(!h3dInit())
    {
        _logger.fatal("h3dInit failed");
    } // end if

    setAnisotropy(anisotropy());

    if(!loadNewResources())
	{
		_logger.fatal("Failed to load default avatar or scene!");
	} // end if

    _avatar = root()->loadModel("models/ares/ares.scene.xml");
    _scene = root()->loadScene("scenes/asteroids/asteroids.scene.xml");

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
