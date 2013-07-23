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
			OIS::Object* oisDevice = _ois->createInputObject(it->first, true, it->second);

			_logger.debug(QString("    - %1 (id: %2; vendor: %3)")
					.arg(describeOISType(oisDevice->type()))
					.arg(oisDevice->getID())
					.arg(QString::fromStdString(oisDevice->vendor()))
					);

			GenericDevice* genDevice = new GenericDevice(
					(InputDriver*) this,
					QString("Device %1").arg(oisDevice->getID())
					);

			/*
			genDevice->addAxisSignal(AxisInputSignal* signal);
			genDevice->addButtonSignal(ButtonInputSignal* signal);
			*/

			OISDriverEventHandler* handler = new OISDriverEventHandler(genDevice, oisDevice);

			switch(oisDevice->type())
			{
				/* We really don't want this.
				case OIS::OISKeyboard:
				{
					OIS::Keyboard* keyboard = (OIS::Keyboard*) oisDevice;
					keyboard->setEventCallback(handler);
					break;
				} // end case
				*/

				case OIS::OISMouse:
				{
					OIS::Mouse* mouse = (OIS::Mouse*) oisDevice;
					mouse->setEventCallback(handler);

					const OIS::MouseState &mouseState = mouse->getMouseState();
					mouseState.width = 100;
					mouseState.height = 100;

					_logger.debug(QString("Created mouse event handler for %1.").arg(describeOISType(oisDevice->type())));
					break;
				} // end case

				case OIS::OISJoyStick:
				{
					OIS::JoyStick* joystick = (OIS::JoyStick*) oisDevice;
					joystick->setEventCallback(handler);
					_logger.debug(QString("Created joystick event handler for %1.").arg(describeOISType(oisDevice->type())));
					break;
				} // end case

				/*TODO: Implement?
				case OIS::OISTablet:
				case OIS::OISMultiTouch:
				*/

				default:
					_logger.warn(QString("Unrecognized device type %1 for id=%2, vendor=%3!")
							.arg(describeOISType(oisDevice->type()))
							.arg(oisDevice->getID())
							.arg(QString::fromStdString(oisDevice->vendor()))
							);
			} // end switch

			_devices[oisDevice] = handler;
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
			switch(oisDevice->type())
			{
				/* Unused currently.
				case OIS::OISKeyboard:
				{
					OIS::Keyboard* keyboard = (OIS::Keyboard*) oisDevice;
					keyboard->setEventCallback(NULL);
					break;
				} // end case
				*/

				case OIS::OISMouse:
				{
					OIS::Mouse* mouse = (OIS::Mouse*) oisDevice;
					mouse->setEventCallback(NULL);
					break;
				} // end case

				case OIS::OISJoyStick:
				{
					OIS::JoyStick* joystick = (OIS::JoyStick*) oisDevice;
					joystick->setEventCallback(NULL);
					break;
				} // end case

				/*TODO: Implement?
				case OIS::OISTablet:
				case OIS::OISMultiTouch:
				*/

				default:
					_logger.warn(QString("Unrecognized device type %1 for id=%2, vendor=%3!")
							.arg(describeOISType(oisDevice->type()))
							.arg(oisDevice->getID())
							.arg(QString::fromStdString(oisDevice->vendor()))
							);
			} // end switch
			_ois->destroyInputObject(oisDevice);
			delete _devices[oisDevice];
			_devices.remove(oisDevice);
		} // end foreach

		OIS::InputManager::destroyInputSystem(_ois);
	} // end if
} // end teardownOIS


OISDriverEventHandler::OISDriverEventHandler(GenericDevice* device, OIS::Object* oisDevice) :
		device(device),
		oisDevice(oisDevice),
		_logger(PLogManager::getLogger("OISDriverEventHandler")),
		QObject()
{
	startTimer(10);
} // end OISDriverEventHandler

OISDriverEventHandler::~OISDriverEventHandler()
{
	device->deleteLater();
} // end ~OISDriverEventHandler

void OISDriverEventHandler::timerEvent(QTimerEvent* event)
{
	oisDevice->capture();
} // end ~OISDriverEventHandler

bool OISDriverEventHandler::keyPressed(const OIS::KeyEvent& arg)
{
} // end keyPressed

bool OISDriverEventHandler::keyReleased(const OIS::KeyEvent& arg)
{
} // end keyReleased

bool OISDriverEventHandler::mouseMoved(const OIS::MouseEvent& arg)
{
	const OIS::MouseState& s = arg.state;
	_logger.debug(QString("Mouse moved: abs(%1, %2, %3); rel: (%4, %5, %6)")
				  .arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
				  .arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
				  );
} // end mouseMoved

bool OISDriverEventHandler::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	const OIS::MouseState& s = arg.state;
	_logger.debug(QString("Mouse button %1 pressed: abs(%2, %3, %4); rel: (%5, %6, %7)")
			.arg(id)
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
} // end mousePressed

bool OISDriverEventHandler::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	const OIS::MouseState& s = arg.state;
	_logger.debug(QString("Mouse button %1 released: abs(%2, %3, %4); rel: (%5, %6, %7)")
			.arg(id)
			.arg(s.X.abs).arg(s.Y.abs).arg(s.Z.abs)
			.arg(s.X.rel).arg(s.Y.rel).arg(s.Z.rel)
			);
} // end mouseReleased

bool OISDriverEventHandler::buttonPressed(const OIS::JoyStickEvent& arg, int button)
{
} // end buttonPressed

bool OISDriverEventHandler::buttonReleased(const OIS::JoyStickEvent& arg, int button)
{
} // end buttonReleased

bool OISDriverEventHandler::axisMoved(const OIS::JoyStickEvent& arg, int axis)
{
} // end axisMoved

bool OISDriverEventHandler::povMoved(const OIS::JoyStickEvent& arg, int pov)
{
} // end povMoved

bool OISDriverEventHandler::vector3Moved(const OIS::JoyStickEvent& arg, int index)
{
} // end vector3Moved
