#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "qchannels/qchannels.h"

#include "ogreitem.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

	// Register the networking code with QML
	qmlRegisterType<QChannels>("precursors.networking", 1, 0, "QChannels");

	// Register Ogre
    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");

	//QQmlApplicationEngine engine("resources/qml/launcher.qml");
	QQmlApplicationEngine engine("resources/main.qml");

    return app.exec();
} // end main
