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

	Q_PROPERTY(Entity* root READ root)
	Q_PROPERTY(Entity* avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
	Q_PROPERTY(Entity* scene READ scene WRITE setScene NOTIFY sceneChanged)
	Q_PROPERTY(QList<Entity*> skyboxes READ skyboxes WRITE setSkyboxes NOTIFY skyboxesChanged)

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
	QList<Entity*> skyboxes() const;
	QString contentDirs() const;
	QString defaultPipeline() const;
	int anisotropy() const;
	int antiAliasingSamples() const;

	void setAvatar(Entity* avatar);
	void setScene(Entity* scene);
	void setSkyboxes(QList<Entity*> skyboxes);
	void setAnisotropy(int textureAnisotropy);
	void setAntiAliasingSamples(int samples);

	Q_INVOKABLE void addSkybox(Entity* skybox);

	void init();
	void update();
	void printHordeMessages();

	bool loadNewResources();
	H3DRes loadPipeline(QString pipelineName);

signals:
	void avatarChanged(const Entity* avatar);
	void sceneChanged(const Entity* scene);
	void skyboxesChanged(const QList<Entity*> skyboxes);
	void anisotropyChanged(int textureAnisotropy);
	void antiAliasingSamplesChanged(int samples);
	void initFinished();

private:
	Entity* _avatar;
	Entity* _scene;
	QList<Entity*> _skyboxes;

	bool _initialized;

	PLogger& _logger;
	PSettingsManager& _settings;
}; // end Horde3DManager

#endif // HORDE3DMANAGER_H
