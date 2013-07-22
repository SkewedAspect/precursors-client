#include "inputdevice.h"


InputDevice::InputDevice(QObject *parent) :
    QObject(parent)
{
}

QString InputDevice::id()
{
	return _id;
} // end id

InputDriver* InputDevice::driver()
{
	return _driver;
} // end driver

InputDevice::AxisSignalHash InputDevice::axisSignals()
{
	return _axisSignals;
} // end axisSignals

InputDevice::ButtonSignalHash InputDevice::buttonSignals()
{
	return _buttonSignals;
} // end buttonSignals

InputDevice::SignalHash InputDevice::inputSignals()
{
	return _inputSignals;
} // end inputSignals
