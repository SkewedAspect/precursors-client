#include "inputdevice.h"


InputDevice::InputDevice(InputDriver* driver) :
    QObject((QObject*) driver)
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
	SignalHash inputSignals;

	foreach(QString key, _axisSignals.keys())
	{
		inputSignals[key] = (InputSignal*) _axisSignals[key];
	} // end foreach

	foreach(QString key, _buttonSignals.keys())
	{
		inputSignals[key] = (InputSignal*) _buttonSignals[key];
	} // end foreach

	return inputSignals;
} // end inputSignals
