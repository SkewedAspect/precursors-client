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


OISDriver::OISDriver(ControlsManager* manager, QWindow* window) :
		InputDriver(manager, window),
		_logger(PLogManager::getLogger("horde3d")),
		_settings(PSettingsManager::instance())
{
	ois = OIS::InputManager::createInputSystem((std::size_t) window->winId());

	_logger.info(QString("%1 starting; using OIS %2 with input system \"%3\".")
			.arg(name())
			.arg(QString::fromStdString(ois->getVersionName()))
			.arg(QString::fromStdString(ois->inputSystemName()))
			);

	for(int type = OIS::OISUnknown; type <= OIS::OISMultiTouch; ++type)
	{
		int deviceCount = ois->getNumberOfDevices((OIS::Type) type);
		_logger.debug(QString("%1 %2 devices found.")
				.arg(deviceCount)
				.arg(describeOISType((OIS::Type) type))
				);
	} // end for

	// typedef std::multimap<Type, std::string> DeviceList;
	OIS::DeviceList devices = (ois->listFreeDevices());
	for(OIS::DeviceList::const_iterator it = devices.begin(); it != devices.end(); ++it)
	{
		_logger.debug(QString("    %1: %2")
				.arg(describeOISType(it->first))
				.arg(QString::fromStdString(it->second))
				);
	} // end for
} // end OISDriver

OISDriver::~OISDriver()
{
	OIS::InputManager::destroyInputSystem(ois);
} // end ~OISDriver

QString OISDriver::name()
{
	return "OIS Input Driver";
} // end name
