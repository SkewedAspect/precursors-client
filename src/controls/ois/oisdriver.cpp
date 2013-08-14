#include <QWindow>

#include "oisdriver.h"
#include "controls/devices/genericdevice.h"
#include "controls/signals/axisinputsignal.h"
#include "controls/signals/buttoninputsignal.h"


QString describeOISType(OIS::Type type)
{
	switch(type)
	{
		case OIS::OISUnknown:
			return "Unknown";
		case OIS::OISKeyboard:
			return "Keyboard";
		case OIS::OISMouse:
			return "Mouse";
		case OIS::OISJoyStick:
			return "JoyStick";
		case OIS::OISTablet:
			return "Tablet";
		case OIS::OISMultiTouch:
			return "MultiTouch";
	} // end switch
} // end describeOISType


OISDriver::OISDriver() :
		_ois(NULL),
		_logger(PLogManager::getLogger("OISDriver")),
		_settings(PSettingsManager::instance()),
		InputDriver()
{
} // end OISDriver

OISDriver::~OISDriver()
{
	teardownOIS();
} // end ~OISDriver

QString OISDriver::name()
{
	return "OIS Input Driver";
} // end name

void OISDriver::setWindow(QWindow* window)
{
	_logger.info(QString("OISDriver::setWindow(%1)").arg((std::size_t) window));

	teardownOIS();

	try
	{
		_ois = OIS::InputManager::createInputSystem((std::size_t) window->winId());

		_logger.info(QString("%1 starting; using OIS %2 with input system \"%3\".")
				.arg(name())
				.arg(QString::fromStdString(_ois->getVersionName()))
				.arg(QString::fromStdString(_ois->inputSystemName()))
				);

		for(int type = OIS::OISUnknown; type <= OIS::OISMultiTouch; ++type)
		{
			int deviceCount = _ois->getNumberOfDevices((OIS::Type) type);

			_logger.debug(QString("%1 %2 devices found.")
					.arg(deviceCount)
					.arg(describeOISType((OIS::Type) type))
					);
		} // end for

		// typedef std::multimap<Type, std::string> DeviceList;
		OIS::DeviceList availableDevices = (_ois->listFreeDevices());
		for(OIS::DeviceList::const_iterator it = availableDevices.begin(); it != availableDevices.end(); ++it)
		{
			/*
			genDevice->addAxisSignal(AxisInputSignal* signal);
			genDevice->addButtonSignal(ButtonInputSignal* signal);
			*/

			switch(it->first)
			{
				/* We really don't want this, unless we can easily switch keyboard input between this and Qt itself.
				case OIS::OISKeyboard:
				{
					OIS::Keyboard* keyboard = (OIS::Keyboard*) oisDevice;
					keyboard->setEventCallback(handler);
					break;
				} // end case
				*/

				/*
				case OIS::OISMouse:
				{
					OIS::Object* oisDevice = _ois->createInputObject(it->first, true, it->second);

					_logger.debug(QString("    - %1 (id: %2; vendor: %3)")
							.arg(describeOISType(oisDevice->type()))
							.arg(oisDevice->getID())
							.arg(QString::fromStdString(oisDevice->vendor()))
							);

					_devices[oisDevice] = new OISMouseEventHandler(this, (OIS::Mouse*) oisDevice);

					_logger.debug(QString("Created mouse event handler for %1.").arg(describeOISType(oisDevice->type())));
					break;
				} // end case
				*/

				case OIS::OISJoyStick:
				{
					OIS::Object* oisDevice = _ois->createInputObject(it->first, true, it->second);

					_logger.debug(QString("    - %1 (id: %2; vendor: %3)")
							.arg(describeOISType(oisDevice->type()))
							.arg(oisDevice->getID())
							.arg(QString::fromStdString(oisDevice->vendor()))
							);

					_devices[oisDevice] = new OISJoystickEventHandler(this, (OIS::JoyStick*) oisDevice);
					emit deviceAttached(_devices[oisDevice]->_device);

					_logger.debug(QString("Created joystick event handler for %1.").arg(describeOISType(oisDevice->type())));
					break;
				} // end case

				/*TODO: Implement?
				case OIS::OISTablet:
				case OIS::OISMultiTouch:
				*/

				default:
					_logger.warn(QString("Unsupported device type %1 for vendor=%2!")
							.arg(describeOISType(it->first))
							.arg(QString::fromStdString(it->second))
							);
			} // end switch
		} // end for
	}
	catch(std::exception& exc)
	{
		_logger.fatal(QString("Exception caught while initializing OIS! %1").arg(exc.what()));
	} // end try

	_window = window;
} // end setWindow

void OISDriver::teardownOIS()
{
	_logger.info("OISDriver::teardownOIS()");

	if(_ois)
	{
		foreach(OIS::Object* oisDevice, _devices.keys())
		{
			_ois->destroyInputObject(oisDevice);
			delete _devices[oisDevice];
			_devices.remove(oisDevice);
		} // end foreach

		OIS::InputManager::destroyInputSystem(_ois);
	} // end if
} // end teardownOIS


//-------------------------------------------------------------------------------------------------------------------//

OISDriverEventHandler::OISDriverEventHandler(QString name) :
		name(name),
		QObject()
{
} // end OISDriverEventHandler

OISDriverEventHandler::~OISDriverEventHandler()
{
} // end ~OISDriverEventHandler


//-------------------------------------------------------------------------------------------------------------------//

OISMouseEventHandler::OISMouseEventHandler(InputDriver* driver, OIS::Mouse* oisDevice) :
		OISDriverEventHandler(QString("%1 (Mouse)").arg(QString::fromStdString(oisDevice->vendor()))),
		_oisDevice(oisDevice),
		_logger(PLogManager::getLogger(QString("OISMouseEventHandler.%1").arg(name)))
{
	_device = new GenericDevice(driver, InputDevice::MOUSE, name);

	const OIS::MouseState &mouseState = _oisDevice->getMouseState();
	mouseState.width = 100;
	mouseState.height = 100;

	_oisDevice->setEventCallback(this);
	startTimer(10);
} // end OISMouseEventHandler

OISMouseEventHandler::~OISMouseEventHandler()
{
	_oisDevice->setEventCallback(NULL);
	_device->deleteLater();
} // end ~OISMouseEventHandler

void OISMouseEventHandler::timerEvent(QTimerEvent* event)
{
	_oisDevice->capture();
} // end ~OISMouseEventHandler

bool OISMouseEventHandler::mouseMoved(const OIS::MouseEvent& event)
{
	const OIS::MouseState& s = event.state;
	_logger.debug(QString("Mouse moved: abs(%1, %2, %3); rel: (%4, %5, %6)")
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
	return true;
} // end mouseMoved

bool OISMouseEventHandler::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
	const OIS::MouseState& s = event.state;
	_logger.debug(QString("Mouse button %1 pressed: abs(%2, %3, %4); rel: (%5, %6, %7)")
			.arg(id)
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
	return true;
} // end mousePressed

bool OISMouseEventHandler::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
	const OIS::MouseState& s = event.state;
	_logger.debug(QString("Mouse button %1 released: abs(%2, %3, %4); rel: (%5, %6, %7)")
			.arg(id)
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
	return true;
} // end mouseReleased


//-------------------------------------------------------------------------------------------------------------------//

struct OISJoystickEventHandler::Hat
{
	enum HatDirection {
		North,
		South,
		East,
		West
	};

	QString name;
	int index;
	QList<ButtonInputSignal*> _signals;
	int lastDirection;

	Hat(GenericDevice* device, int index) :
			index(index),
			lastDirection(0),
			name(QString("POV Hat %1").arg(index + 1))
	{
		_signals.append(new ButtonInputSignal(device, QString("%1 North").arg(name)));
		_signals.append(new ButtonInputSignal(device, QString("%1 South").arg(name)));
		_signals.append(new ButtonInputSignal(device, QString("%1 East").arg(name)));
		_signals.append(new ButtonInputSignal(device, QString("%1 West").arg(name)));
	} // end Hat

	void changed(int direction)
	{
		int changes = lastDirection ^ direction;

		if(changes != 0)
		{
#define CHECK_DIRECTION(DIRECTION) if(changes & OIS::Pov::DIRECTION) { _signals[DIRECTION]->emitUpdated(direction & OIS::Pov::DIRECTION); }
			CHECK_DIRECTION(North)
			CHECK_DIRECTION(South)
			CHECK_DIRECTION(East)
			CHECK_DIRECTION(West)
#undef CHECK_DIRECTION
		} // end if
	} // end changed
}; // end Hat

OISJoystickEventHandler::OISJoystickEventHandler(InputDriver* driver, OIS::JoyStick* oisDevice) :
		OISDriverEventHandler(QString::fromStdString(oisDevice->vendor())),
		_oisDevice(oisDevice),
		_logger(PLogManager::getLogger(QString("OISJoystickEventHandler.%1").arg(name)))
{
	_device = new GenericDevice(driver, InputDevice::JOYSTICK, name);

	OIS::JoyStickState state = _oisDevice->getJoyStickState();

	for(std::vector<OIS::Axis>::iterator i = state.mAxes.begin(), e = state.mAxes.end(); i != e; ++i)
	{
		QString axisName = QString("Axis %1").arg(_axes.length() + 1);
		AxisInputSignal* axisSignal = new AxisInputSignal(_device, axisName, axisName);
		_axes.append(axisSignal);
		_device->addAxisSignal(axisSignal);
	} // end for

	for(std::vector<bool>::iterator i = state.mButtons.begin(), e = state.mButtons.end(); i != e; ++i)
	{
		QString buttonName = QString("Button %1").arg(_buttons.length() + 1);
		ButtonInputSignal* buttonSignal = new ButtonInputSignal(_device, buttonName, buttonName);
		_buttons.append(buttonSignal);
		_device->addButtonSignal(buttonSignal);
	} // end for

	for(int idx = 0; idx < 4; ++idx)
	{
		Hat* hat = new Hat(_device, idx);
		for(int dir = 0; dir < 4; ++dir)
		{
			_device->addButtonSignal(hat->_signals[dir]);
		} // end for
		_hats.append(hat);
	} // end for

	//FIXME: POV hats!
	//FIXME: Sliders! (???)
	//FIXME: Vector3s!

	_oisDevice->setEventCallback(this);
	startTimer(10);
} // end OISJoystickEventHandler

OISJoystickEventHandler::~OISJoystickEventHandler()
{
	_oisDevice->setEventCallback(NULL);
	_device->deleteLater();
} // end ~OISJoystickEventHandler

void OISJoystickEventHandler::timerEvent(QTimerEvent* event)
{
	_oisDevice->capture();
} // end ~OISJoystickEventHandler

bool OISJoystickEventHandler::buttonPressed(const OIS::JoyStickEvent& event, int buttonIndex)
{
	_buttons[buttonIndex]->emitUpdated(true);
	return true;
} // end buttonPressed

bool OISJoystickEventHandler::buttonReleased(const OIS::JoyStickEvent& event, int buttonIndex)
{
	_buttons[buttonIndex]->emitUpdated(false);
	return true;
} // end buttonReleased

bool OISJoystickEventHandler::axisMoved(const OIS::JoyStickEvent& event, int axisIndex)
{
	_axes[axisIndex]->emitUpdated(event.state.mAxes[axisIndex].abs);

	return true;
} // end axisMoved

bool OISJoystickEventHandler::sliderMoved(const OIS::JoyStickEvent& event, int sliderIndex)
{
	_logger.debug(QString("Slider %1: (%2, %3)")
			.arg(sliderIndex)
			.arg(event.state.mSliders[sliderIndex].abX)
			.arg(event.state.mSliders[sliderIndex].abY)
			);

	return true;
} // end sliderMoved

bool OISJoystickEventHandler::povMoved(const OIS::JoyStickEvent& event, int povIndex)
{
	_hats[povIndex]->changed(event.state.mPOV[povIndex].direction);
	return true;
} // end povMoved

bool OISJoystickEventHandler::vector3Moved(const OIS::JoyStickEvent& event, int vec3Index)
{
	OIS::Vector3 vec = event.state.mVectors[vec3Index];
	_logger.debug(QString("Orientation %1: (%2, %3, %4)")
			.arg(vec3Index)
			.arg(vec.x).arg(vec.y).arg(vec.z)
			);
	return true;
} // end vector3Moved
