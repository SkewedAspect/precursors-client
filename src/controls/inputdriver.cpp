#include <QWindow>

#include "controlsmanager.h"
#include "inputdriver.h"


InputDriver::InputDriver(ControlsManager* manager, QWindow* window) :
    QObject(window)
{
	connect(this, SIGNAL(deviceAttached(InputDevice*)), manager, SLOT(onDeviceAttached(InputDevice*)));
	connect(this, SIGNAL(deviceDetached(InputDevice*)), manager, SLOT(onDeviceDetached(InputDevice*)));
} // end InputDriver
