#include <QWindow>

#include "oisdriver.h"
#include "../devices/genericdevice.h"


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
		oisDevice(oisDevice),
		_logger(PLogManager::getLogger(QString("OISMouseEventHandler.%1").arg(name)))
{
	device = new GenericDevice(driver, name);

	const OIS::MouseState &mouseState = oisDevice->getMouseState();
	mouseState.width = 100;
	mouseState.height = 100;

	oisDevice->setEventCallback(this);
	startTimer(10);
} // end OISMouseEventHandler

OISMouseEventHandler::~OISMouseEventHandler()
{
	oisDevice->setEventCallback(NULL);
	device->deleteLater();
} // end ~OISMouseEventHandler

void OISMouseEventHandler::timerEvent(QTimerEvent* event)
{
	oisDevice->capture();
} // end ~OISMouseEventHandler

bool OISMouseEventHandler::mouseMoved(const OIS::MouseEvent& arg)
{
	const OIS::MouseState& s = arg.state;
	_logger.debug(QString("Mouse moved: abs(%1, %2, %3); rel: (%4, %5, %6)")
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
	return true;
} // end mouseMoved

bool OISMouseEventHandler::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	const OIS::MouseState& s = arg.state;
	_logger.debug(QString("Mouse button %1 pressed: abs(%2, %3, %4); rel: (%5, %6, %7)")
			.arg(id)
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
	return true;
} // end mousePressed

bool OISMouseEventHandler::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	const OIS::MouseState& s = arg.state;
	_logger.debug(QString("Mouse button %1 released: abs(%2, %3, %4); rel: (%5, %6, %7)")
			.arg(id)
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
	return true;
} // end mouseReleased


//-------------------------------------------------------------------------------------------------------------------//

OISJoystickEventHandler::OISJoystickEventHandler(InputDriver* driver, OIS::JoyStick* oisDevice) :
		OISDriverEventHandler(QString::fromStdString(oisDevice->vendor())),
		oisDevice(oisDevice),
		_logger(PLogManager::getLogger(QString("OISJoystickEventHandler.%1").arg(name)))
{
	device = new GenericDevice(driver, name);

	oisDevice->setEventCallback(this);
	startTimer(10);
} // end OISJoystickEventHandler

OISJoystickEventHandler::~OISJoystickEventHandler()
{
	oisDevice->setEventCallback(NULL);
	device->deleteLater();
} // end ~OISJoystickEventHandler

void OISJoystickEventHandler::timerEvent(QTimerEvent* event)
{
	oisDevice->capture();
} // end ~OISJoystickEventHandler

bool OISJoystickEventHandler::buttonPressed(const OIS::JoyStickEvent& arg, int buttonIndex)
{
	_logger.debug(QString("Button %1 pressed.").arg(buttonIndex));
	return true;
} // end buttonPressed

bool OISJoystickEventHandler::buttonReleased(const OIS::JoyStickEvent& arg, int buttonIndex)
{
	_logger.debug(QString("Button %1 released.").arg(buttonIndex));
	return true;
} // end buttonReleased

bool OISJoystickEventHandler::axisMoved(const OIS::JoyStickEvent& arg, int axisIndex)
{
	// Provide a little dead zone
	if(abs(arg.state.mAxes[axisIndex].abs) > 2500 || arg.state.mAxes[axisIndex].abs < -2500)
	{
		_logger.debug(QString("Axis %1: %2")
				.arg(axisIndex)
				.arg(arg.state.mAxes[axisIndex].abs)
				);
	} // end if

	return true;
} // end axisMoved

bool OISJoystickEventHandler::povMoved(const OIS::JoyStickEvent& arg, int povIndex)
{
	int dir = arg.state.mPOV[povIndex].direction;
	_logger.debug(QString("POV %1: %2%3%4")
			.arg(povIndex)
			.arg(dir & OIS::Pov::North ? "North" : (dir & OIS::Pov::South ? "South" : ""))
			.arg(dir & OIS::Pov::East ? "East" : (dir & OIS::Pov::West ? "West" : ""))
			.arg(dir & OIS::Pov::Centered ? "Centered" : "")
			);
	return true;
} // end povMoved

bool OISJoystickEventHandler::vector3Moved(const OIS::JoyStickEvent& arg, int vec3Index)
{
	OIS::Vector3 vec = arg.state.mVectors[vec3Index];
	_logger.debug(QString("Orientation %1: (%2, %3, %4)")
			.arg(vec3Index)
			.arg(vec.x).arg(vec.y).arg(vec.z)
			);
	return true;
} // end vector3Moved
