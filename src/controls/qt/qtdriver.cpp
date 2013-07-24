#include <cmath>

#include <QMouseEvent>
#include <QScreen>
#include <QWindow>

#include "controls/devices/genericdevice.h"
#include "controls/signals/axisinputsignal.h"
#include "controls/signals/buttoninputsignal.h"
#include "qtdriver.h"


QtDriver::QtDriver() :
		InputDriver(),
		_mouseDevice(NULL),
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
	if(!_mouseDevice)
	{
		_mouseDevice = new GenericDevice(this, "Dumb Mouse");

		_mouseAxes["X"] = new AxisInputSignal(_mouseDevice, "X", "horizontal movement");
		_mouseAxes["Y"] = new AxisInputSignal(_mouseDevice, "Y", "vertical movement");

		_mouseDevice->addAxisSignal(_mouseAxes["X"]);
		_mouseDevice->addAxisSignal(_mouseAxes["Y"]);

#define BTN(BUTTON, NAME, DESC)	_mouseButtons[Qt::BUTTON] = new ButtonInputSignal(_mouseDevice, NAME, DESC)
		BTN(LeftButton, "Left", "The left button. (The left button may be the right button on left-handed mice.)");
		BTN(RightButton, "Right", "The right button.");
		BTN(MidButton, "Middle", "The middle button.");
		BTN(BackButton, "Back", "The 'Back' button. (Typically present on the 'thumb' side of a mouse with extra buttons. This is NOT the tilt wheel.)");
		BTN(ForwardButton, "Forward", "The 'Forward' button. (Typically present beside the 'Back' button, and also pressed by the thumb.)");
		BTN(TaskButton, "Task", "The 'Task' button.");
		BTN(ExtraButton4, "Extra 4", "The 7th non-wheel mouse button.");
		BTN(ExtraButton5, "Extra 5", "The 8th non-wheel mouse button.");
		BTN(ExtraButton6, "Extra 6", "The 9th non-wheel mouse button.");
		BTN(ExtraButton7, "Extra 7", "The 10th non-wheel mouse button.");
		BTN(ExtraButton8, "Extra 8", "The 11th non-wheel mouse button.");
		BTN(ExtraButton9, "Extra 9", "The 12th non-wheel mouse button.");
		BTN(ExtraButton10, "Extra 10", "The 13th non-wheel mouse button.");
		BTN(ExtraButton11, "Extra 11", "The 14th non-wheel mouse button.");
		BTN(ExtraButton12, "Extra 12", "The 15th non-wheel mouse button.");
		BTN(ExtraButton13, "Extra 13", "The 16th non-wheel mouse button.");
		BTN(ExtraButton14, "Extra 14", "The 17th non-wheel mouse button.");
		BTN(ExtraButton15, "Extra 15", "The 18th non-wheel mouse button.");
		BTN(ExtraButton16, "Extra 16", "The 19th non-wheel mouse button.");
		BTN(ExtraButton17, "Extra 17", "The 20th non-wheel mouse button.");
		BTN(ExtraButton18, "Extra 18", "The 21st non-wheel mouse button.");
		BTN(ExtraButton19, "Extra 19", "The 22nd non-wheel mouse button.");
		BTN(ExtraButton20, "Extra 20", "The 23rd non-wheel mouse button.");
		BTN(ExtraButton21, "Extra 21", "The 24th non-wheel mouse button.");
		BTN(ExtraButton22, "Extra 22", "The 25th non-wheel mouse button.");
		BTN(ExtraButton23, "Extra 23", "The 26th non-wheel mouse button.");
		BTN(ExtraButton24, "Extra 24", "The 27th non-wheel mouse button.");
#undef BTN

		foreach(ButtonInputSignal* button, _mouseButtons.values())
		{
			_mouseDevice->addButtonSignal(button);
		} // end foreach

		emit deviceAttached(_mouseDevice);
	} // end if

	_window = window;

	_screenSize = _window->screen()->size();
	connect(_window, SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));

	connect(_window, SIGNAL(mouseMoved(QMouseEvent*, QPoint)), this, SLOT(onMouseMoved(QMouseEvent*, QPoint)));
	connect(_window, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(onMousePressed(QMouseEvent*)));
	connect(_window, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(onMouseReleased(QMouseEvent*)));
} // end setWindow

void QtDriver::onScreenChanged(QScreen* screen)
{
	_screenSize = screen->size();
} // end onScreenChanged

void QtDriver::onMouseMoved(QMouseEvent* event, QPoint screenDelta)
{
	float divisor = fmax(_screenSize.width(), _screenSize.height());

	float deltaX = screenDelta.x() / divisor;
	float deltaY = screenDelta.y() / divisor;

	_logger.debug(QString("Mouse moved: %1, %2 (%3, %4)")
			.arg(deltaX).arg(deltaY)
			.arg(screenDelta.x()).arg(screenDelta.y())
			);

	_mouseAxes["X"]->emitUpdated(deltaX);
	_mouseAxes["Y"]->emitUpdated(deltaY);
} // end onMouseMoved

void QtDriver::onMousePressed(QMouseEvent* event)
{
	_logger.debug(QString("Mouse button %1 pressed.")
			.arg(event->button())
			);
	_mouseButtons[event->button()]->emitUpdated(true);
} // end onMousePressed

void QtDriver::onMouseReleased(QMouseEvent* event)
{
	_logger.debug(QString("Mouse button %1 released.")
			.arg(event->button())
			);
	_mouseButtons[event->button()]->emitUpdated(false);
} // end onMouseReleased
