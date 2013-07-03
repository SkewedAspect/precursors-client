#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtGui/QFontDatabase>

#include "qchannels/qchannels.h"
#include "qchannels/qchannelsrequest.h"

#include "ogreitem.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

	// Register application fonts
	//TODO: Figure out why this doesn't work on OSX on Qt 5.1
	QFontDatabase::addApplicationFont("resources/fonts/trajan.otf");
	QFontDatabase::addApplicationFont("resources/fonts/titillium.ttf");

	// Register the networking code with QML
	qmlRegisterType<QChannels>("Precursors.Networking", 1, 0, "QChannels");
	qmlRegisterType<QChannelsRequest>("Precursors.Networking", 1, 0, "QChannelsRequest");

	// Register Ogre
    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");

	QQmlApplicationEngine engine("resources/qml/launcher.qml");
    return app.exec();
} // end main
