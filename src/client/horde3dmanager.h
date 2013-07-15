#ifndef HORDE3DMANAGER_H
#define HORDE3DMANAGER_H

#include <QtQuick/QQuickWindow>
#include <QtCore/QTime>

#include <Horde3D.h>

#include "plogging/plogging.h"
#include "psettings/psettings.h"


#define CFG_CONTENT_DIRS "horde3d.contentDirs"
#define DEFAULT_CONTENT_DIRS "resources/content"

#define CFG_ANISOTROPY "horde3d.anisotropy"
#define DEFAULT_ANISOTROPY 8

#define CFG_AA_SAMPLES "horde3d.antiAliasingSamples"
#define DEFAULT_AA_SAMPLES 0

#define CFG_DEFAULT_PIPELINE "horde3d.defaultPipeline"
#define DEFAULT_DEFAULT_PIPELINE "pipelines/forward.pipeline.xml"


class QSGSimpleTextureNode;
class QOpenGLFramebufferObject;

class Entity;


class Horde3DManager : public QObject
{
	Q_OBJECT

	typedef QMultiHash<QString, Entity*> EntityHash;

	Q_PROPERTY(Entity* root READ root)
	Q_PROPERTY(Entity* avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
	Q_PROPERTY(Entity* scene READ scene WRITE setScene NOTIFY sceneChanged)
	Q_PROPERTY(EntityHash skyboxes READ skyboxes NOTIFY skyboxesChanged)

	Q_PROPERTY(QString contentDirs READ contentDirs)
	Q_PROPERTY(QString defaultPipeline READ defaultPipeline)

	Q_PROPERTY(int anisotropy READ anisotropy WRITE setAnisotropy NOTIFY anisotropyChanged)
	Q_PROPERTY(int antiAliasingSamples READ antiAliasingSamples WRITE setAntiAliasingSamples NOTIFY antiAliasingSamplesChanged)

protected:
	Horde3DManager(QObject* parent = 0);

public:
	~Horde3DManager();

	static Horde3DManager& instance();

	Entity* root() const;

	Entity* avatar() const;
	Entity* scene() const;
	QMultiHash<QString, Entity*> skyboxes() const;
	QString contentDirs() const;
	QString defaultPipeline() const;
	int anisotropy() const;
	int antiAliasingSamples() const;

	void setAvatar(Entity* avatar);
	void setScene(Entity* scene);
	void setAnisotropy(int textureAnisotropy);
	void setAntiAliasingSamples(int samples);

	Q_INVOKABLE bool loadResource(H3DResTypes::List type, QString path, int flags, H3DRes* resRef = NULL);

	Q_INVOKABLE bool isSceneLoaded(QString sceneID);
	Q_INVOKABLE Entity* getScene(QString sceneID);
	Q_INVOKABLE bool loadScene(QString sceneID, int flags = 0);
	Q_INVOKABLE bool switchScene(QString sceneID, bool keep = false);
	Q_INVOKABLE void unloadScene(QString sceneID);
	Q_INVOKABLE void renderScene(Entity* camera, QString sceneID = QString());

	Q_INVOKABLE void addSkybox(QString sceneID, Entity* skybox);

	void init();
	void update();
	void printHordeMessages();

	bool loadNewResources();
	H3DRes loadPipeline(QString pipelineName);

protected:
	void timerEvent(QTimerEvent* event);

signals:
	void avatarChanged(const Entity* avatar);
	void sceneChanged(const Entity* scene);
	void skyboxesChanged(const QMultiHash<QString, Entity*> skyboxes);
	void anisotropyChanged(int textureAnisotropy);
	void antiAliasingSamplesChanged(int samples);
	void initFinished();
	void sceneLoaded(QString sceneID, const Entity* scene);
	void resourceLoaded(H3DRes resource);

private slots:
	void onResourceLoaded(H3DRes resource);

private:
	Entity* _avatar;
	Entity* _scene;
	QMultiHash<QString, Entity*> _skyboxes;
	QHash<QString, Entity*> _loadedScenes;
	QHash<H3DRes, QString> _queuedScenes;
	QList<H3DRes> _queuedResources;

	bool _initialized;
	int _loadTimer;

	PLogger& _logger;
	PSettingsManager& _settings;
}; // end Horde3DManager

#endif // HORDE3DMANAGER_H
