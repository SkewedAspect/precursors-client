#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QFontDatabase>
#include <QtQml/QQmlContext>

#include "qchannels/qchannels.h"
#include "qchannels/qchannelsrequest.h"
#include "psettings/psettings.h"

#include "h3ditem.h"
#include "horde3dwindow.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	// Setup our settings
	PSettingsManager settingsMan;

	// Register application fonts
	//TODO: Figure out why this doesn't work on OSX on Qt 5.1
	QFontDatabase::addApplicationFont("resources/fonts/trajan.otf");
	QFontDatabase::addApplicationFont("resources/fonts/titillium.ttf");

	// Register the networking code with QML
	qmlRegisterType<QChannels>("Precursors.Networking", 1, 0, "QChannels");
	qmlRegisterType<QChannelsRequest>("Precursors.Networking", 1, 0, "QChannelsRequest");

	// Register the settings code with QML
	qmlRegisterType<QChannels>("Precursors.Settings", 1, 0, "PSettingsManager");

	// Register the Horde3D components
	qmlRegisterType<Horde3DItem>("Horde3D", 1, 0, "Horde3DItem");
	qmlRegisterType<Horde3DWindow>("Horde3D", 1, 0, "Horde3DWindow");

	// Setup QML
	QQmlApplicationEngine engine;

	// Put our settings on the root context
	engine.rootContext()->setContextProperty("settingsMan", &settingsMan);
	//engine.rootContext()->setContextProperty("settings", &settingsMan.settings);

	// load our main qml file
	engine.load("resources/qml/launcher.qml");

	return app.exec();
} // end main
