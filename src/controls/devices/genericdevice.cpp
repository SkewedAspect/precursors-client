#include "genericdevice.h"
#include "../signals/axisinputsignal.h"
#include "../signals/buttoninputsignal.h"


GenericDevice::GenericDevice(InputDriver* driver, QString id) :
		InputDevice(driver)
{
	InputDevice::_id = id;
} // end GenericDevice

void GenericDevice::addAxisSignal(AxisInputSignal* signal)
{
	_axisSignals[signal->name()] = signal;
} // end addAxisSignal

void GenericDevice::addButtonSignal(ButtonInputSignal* signal)
{
	_buttonSignals[signal->name()] = signal;
} // end addButtonSignal
