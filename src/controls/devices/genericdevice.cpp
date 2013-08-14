#include "genericdevice.h"
#include "../signals/axisinputsignal.h"
#include "../signals/buttoninputsignal.h"


GenericDevice::GenericDevice(InputDriver* driver, InputDevice::Type type, QString id) :
		_type(type),
		InputDevice(driver)
{
	InputDevice::_id = id;
} // end GenericDevice

const InputDevice::Type GenericDevice::type() const
{
	return _type;
} // end type

PLogger& GenericDevice::_logger() const
{
	return PLogManager::getLogger(QString("GenericDevice[%1][%2]").arg(InputDevice::typeString(_type)).arg(_id));
} // end _logger

void GenericDevice::addAxisSignal(AxisInputSignal* signal)
{
	_axisSignals[signal->name()] = signal;
} // end addAxisSignal

void GenericDevice::addButtonSignal(ButtonInputSignal* signal)
{
	_buttonSignals[signal->name()] = signal;
} // end addButtonSignal
