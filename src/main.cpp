#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "qchannels/qchannels.h"

#include "h3ditem.h"
#include "horde3dwindow.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	// Register the networking code with QML
	qmlRegisterType<QChannels>("Precursors.Networking", 1, 0, "QChannels");

	// Register the Horde3D components
	qmlRegisterType<Horde3DItem>("Horde3D", 1, 0, "Horde3DItem");
	qmlRegisterType<Horde3DWindow>("Horde3D", 1, 0, "Horde3DWindow");

	//QQmlApplicationEngine engine("resources/qml/launcher.qml");
	QQmlApplicationEngine engine("resources/main.qml");

	return app.exec();
} // end main
