#include <QWindow>

#include "controlsmanager.h"
#include "inputdriver.h"


InputDriver::InputDriver() :
		_manager(NULL),
		_window(NULL),
		_logger(PLogManager::getLogger("InputDriver")),
		QObject()
{
} // end InputDriver

ControlsManager* InputDriver::manager()
{
	return _manager;
} // end manager

QWindow* InputDriver::window()
{
	return _window;
} // end window

bool InputDriver::activate(ControlsManager* manager)
{
	if(_manager)
	{
		_logger.error("activate() failed: ControlsManager instance already set!");
		return false;
	} // end if

	_manager = manager;

	connect(this, SIGNAL(deviceAttached(InputDevice*)), _manager, SLOT(onDeviceAttached(InputDevice*)));
	connect(this, SIGNAL(deviceDetached(InputDevice*)), _manager, SLOT(onDeviceDetached(InputDevice*)));

	return true;
} // end activate

bool InputDriver::deactivate()
{
	if(!_manager)
	{
		_logger.error("deactivate() failed: ControlsManager instance not set!");
		return false;
	} // end if

	disconnect(_manager);

	_manager = NULL;

	return true;
} // end deactivate
