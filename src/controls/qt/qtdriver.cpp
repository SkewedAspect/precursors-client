#include <cmath>

#include <QMouseEvent>
#include <QScreen>
#include <QWindow>

#include "qtdriver.h"


QtDriver::QtDriver() :
		InputDriver(),
		_logger(PLogManager::getLogger("QtDriver")),
		_settings(PSettingsManager::instance())
{
} // end QtDriver

QString QtDriver::name()
{
	return "Qt Input Driver";
} // end name

void QtDriver::setWindow(QWindow* window)
{
	_window = window;

	screenSize = _window->screen()->size();
	connect(_window, SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));

	connect(_window, SIGNAL(mouseMoved(QMouseEvent*, QPoint)), this, SLOT(onMouseMoved(QMouseEvent*, QPoint)));
	connect(_window, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(onMousePressed(QMouseEvent*)));
	connect(_window, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(onMouseReleased(QMouseEvent*)));
} // end setWindow

void QtDriver::onScreenChanged(QScreen* screen)
{
	screenSize = screen->size();
} // end onScreenChanged

void QtDriver::onMouseMoved(QMouseEvent* event, QPoint screenDelta)
{

	float divisor = fmax(screenSize.width(), screenSize.height());

	float deltaX = screenDelta.x() / divisor;
	float deltaY = screenDelta.y() / divisor;

	_logger.debug(QString("Mouse moved: %1, %2 (%3, %4)")
			.arg(deltaX).arg(deltaY)
			.arg(screenDelta.x()).arg(screenDelta.y())
			);
} // end onMouseMoved

void QtDriver::onMousePressed(QMouseEvent* event)
{
	_logger.debug(QString("Mouse button %1 pressed.")
			.arg(event->button())
			);
} // end onMousePressed

void QtDriver::onMouseReleased(QMouseEvent* event)
{
	_logger.debug(QString("Mouse button %1 released.")
			.arg(event->button())
			);
} // end onMouseReleased
