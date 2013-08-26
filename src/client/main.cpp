#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QFontDatabase>
#include <QtQml/QQmlContext>

#include "preutil/preutil.h"
#include "plogging/plogging.h"
#include "pchannels/pchannels.h"
#include "pchannels/pchannelsrequest.h"
#include "psettings/psettings.h"
#include "controls/controlsmanager.h"
#include "controls/controlcontext.h"
#include "controls/slots/analogcontrolslot.h"
#include "controls/slots/digitalcontrolslot.h"

#include "entity.h"
#include "horde3ditem.h"
#include "horde3dmanager.h"
#include "horde3dwindow.h"
#include "math3d.h"
#include "timestamp_sync.h"

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
	ControlsManager& controls = ControlsManager::instance();
    Horde3DManager& horde3d = Horde3DManager::instance();
    TimestampSync& timestamp = TimestampSync::instance();
	Math3D math3d;

	// Load controls contexts
	if(!controls.loadContextDefs("resources/contexts.json"))
	{
		return -1;
	} // end if

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

    qmlRegisterType<ControlContext>();
    qmlRegisterType<AnalogControlSlot>();
    qmlRegisterType<DigitalControlSlot>();

	qmlRegisterUncreatableType<Math3D, 1>("Precursors", 1, 0, "Math3D", "Math3D cannot be instantiated.");

    // Setup QML
    QQmlApplicationEngine engine;

    // Put our singletons on the root context
    engine.rootContext()->setContextProperty("settings", &settings);
    engine.rootContext()->setContextProperty("logMan", &logMan);
    engine.rootContext()->setContextProperty("networking", &networking);
    engine.rootContext()->setContextProperty("utils", &utils);
    engine.rootContext()->setContextProperty("controls", &controls);
    engine.rootContext()->setContextProperty("horde3d", &horde3d);
    engine.rootContext()->setContextProperty("timestamp", &timestamp);
    engine.rootContext()->setContextProperty("math3d", &math3d);

    // load our main qml file
    engine.load("resources/qml/launcher.qml");

    return app.exec();
} // end main
