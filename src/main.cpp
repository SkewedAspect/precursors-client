#define USE_QAPPLICATION 1


#include "ogreitem.h"

#ifdef USE_QAPPLICATION
#	include <QtWidgets/QApplication>
#else
#	include <QtGui/QGuiApplication>
#endif

#include <QtQml/QQmlApplicationEngine>

#include "qchannels/qchannels.h"

int main(int argc, char **argv)
{
#ifdef USE_QAPPLICATION
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif
	// Register the networking code with QML
	qmlRegisterType<QChannels>("precursors.networking", 1, 0, "QChannels");

	// Register Ogre
    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");

	//QQmlApplicationEngine engine("resources/qml/launcher.qml");
	QQmlApplicationEngine engine("resources/main.qml");

    return app.exec();
} // end main
