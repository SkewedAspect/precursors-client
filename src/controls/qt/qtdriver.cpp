#include <QWindow>

#include "qtdriver.h"


QtDriver::QtDriver() :
    InputDriver()
{
} // end QtDriver

QString QtDriver::name()
{
	return "Qt Input Driver";
} // end name

void QtDriver::setWindow(QWindow* window)
{
	_window = window;
} // end setWindow
