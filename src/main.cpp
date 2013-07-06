#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QFontDatabase>
#include <QtQml/QQmlContext>

#include "preutil/preutil.h"
#include "plogging/plogging.h"
#include "pchannels/pchannels.h"
#include "pchannels/pchannelsrequest.h"
#include "psettings/psettings.h"

#include "h3ditem.h"
#include "horde3dwindow.h"


int main(int argc, char **argv)
{
	PLogger logger("app");

	QApplication app(argc, argv);
	app.setApplicationName("Precursors QML Client");
	app.setApplicationVersion("0.5.0-alpha1");
	app.setOrganizationDomain("skewedaspect.com");
	app.setOrganizationName("Skewed Aspect");

	logger.info("Starting " + QString("%1 v%2").arg(app.applicationName()).arg(app.applicationVersion()));

	// Setup our settings
	PSettingsManager& settings = PSettingsManager::instance();
	PChannels& networking = PChannels::instance();
	PreUtil& utils = PreUtil::instance();

	// Register application fonts
	QFontDatabase::addApplicationFont("resources/fonts/trajan.otf");
	QFontDatabase::addApplicationFont("resources/fonts/titillium.ttf");

	// Register the networking code with QML
	qmlRegisterType<PChannels>("Precursors", 1, 0, "PreUtil");

	// Register the networking code with QML
	qmlRegisterType<PChannels>("Precursors.Networking", 1, 0, "PChannels");
	qmlRegisterType<PChannelsRequest>("Precursors.Networking", 1, 0, "PChannelsRequest");

	// Register the Horde3D components
	qmlRegisterType<Horde3DItem>("Horde3D", 1, 0, "Horde3DItem");
	qmlRegisterType<Horde3DWindow>("Horde3D", 1, 0, "Horde3DWindow");

	// Setup QML
	QQmlApplicationEngine engine;

	// Put our settings on the root context
	engine.rootContext()->setContextProperty("settings", &settings);
	engine.rootContext()->setContextProperty("networking", &networking);
	engine.rootContext()->setContextProperty("utils", &utils);

	// load our main qml file
	engine.load("resources/qml/launcher.qml");

	return app.exec();
} // end main
