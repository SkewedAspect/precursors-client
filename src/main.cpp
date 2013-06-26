#define USE_QAPPLICATION 1


#include "ogreitem.h"

#ifdef USE_QAPPLICATION
#	include <QtWidgets/QApplication>
#else
#	include <QtGui/QGuiApplication>
#endif

#include <QtQml/QQmlApplicationEngine>

int main(int argc, char **argv)
{
#ifdef USE_QAPPLICATION
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif

    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");

	QQmlApplicationEngine engine("resources/main.qml");

	/*
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("resources/example.qml"));
    view.rootContext()->setContextProperty("Window", &view);
    view.show();
    view.raise();
	*/

    return app.exec();
} // end main
