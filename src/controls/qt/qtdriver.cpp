#include <QWindow>

#include "qtdriver.h"


QtDriver::QtDriver(ControlsManager* manager, QWindow* window) :
    InputDriver(manager, window)
{
} // end QtDriver

QString QtDriver::name()
{
	return "Qt Input Driver";
} // end name
