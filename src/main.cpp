#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "qchannels/qchannels.h"

#include "h3ditem.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	// Register the networking code with QML
	qmlRegisterType<QChannels>("precursors.networking", 1, 0, "QChannels");

	// Register Horde3D
	qmlRegisterType<Horde3DItem>("Horde3D", 1, 0, "Horde3DItem");

	//QQmlApplicationEngine engine("resources/qml/launcher.qml");
	QQmlApplicationEngine engine("resources/main.qml");

	return app.exec();
} // end main
