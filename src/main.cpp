#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QFontDatabase>
#include <QtQml/QQmlContext>

#include "preutil/preutil.h"
#include "plogging/plogging.h"
#include "pchannels/pchannels.h"
#include "pchannels/pchannelsrequest.h"
#include "psettings/psettings.h"

#include "entity.h"
#include "h3ditem.h"
#include "horde3dwindow.h"


int main(int argc, char **argv)
{
	PLogger logger("app");

	QGuiApplication app(argc, argv);
	app.setApplicationName("Precursors QML Client");
	app.setApplicationVersion("0.5.0-alpha1");
	app.setOrganizationDomain("skewedaspect.com");
	app.setOrganizationName("Skewed Aspect");

	logger.info("Starting " + QString("%1 v%2").arg(app.applicationName()).arg(app.applicationVersion()));

	// Setup our managers
	PSettingsManager& settings = PSettingsManager::instance();
	PChannels& networking = PChannels::instance();
	PreUtil& utils = PreUtil::instance();
	PLogManager& logMan = PLogManager::instance();

	// Register application fonts
	QFontDatabase::addApplicationFont("resources/fonts/trajan.otf");
	QFontDatabase::addApplicationFont("resources/fonts/titillium.ttf");

	// Register custom types with QML
	qmlRegisterType<Entity>("Precursors", 1, 0, "Entity");

	qmlRegisterType<PLogger>("Precursors.Logging", 1, 0, "PLogger");

	qmlRegisterType<PChannels>("Precursors.Networking", 1, 0, "PChannels");
	qmlRegisterType<PChannelsRequest>("Precursors.Networking", 1, 0, "PChannelsRequest");

	qmlRegisterType<Horde3DItem>("Horde3D", 1, 0, "Horde3DItem");
	qmlRegisterType<Horde3DWindow>("Horde3D", 1, 0, "Horde3DWindow");

	// Setup QML
	QQmlApplicationEngine engine;

	// Put our settings on the root context
	engine.rootContext()->setContextProperty("settings", &settings);
	engine.rootContext()->setContextProperty("logMan", &logMan);
	engine.rootContext()->setContextProperty("networking", &networking);
	engine.rootContext()->setContextProperty("utils", &utils);

	// load our main qml file
	engine.load("resources/qml/launcher.qml");

	return app.exec();
} // end main
