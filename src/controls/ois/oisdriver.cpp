#include <QWindow>

#include <OIS.h>

#include "oisdriver.h"


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
			OIS::Object* device = _ois->createInputObject(it->first, true, it->second);

			_logger.debug(QString("    - %1 (id: %2; vendor: %3)")
					.arg(describeOISType(device->type()))
					.arg(device->getID())
					.arg(QString::fromStdString(device->vendor()))
					);

			devices.append(device);
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
		while(!devices.isEmpty())
		{
			_ois->destroyInputObject(devices.takeFirst());
		} // end while

		OIS::InputManager::destroyInputSystem(_ois);
	} // end if
} // end teardownOIS
