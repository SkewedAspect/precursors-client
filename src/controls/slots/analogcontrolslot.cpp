#include "analogcontrolslot.h"


AnalogControlSlot::AnalogControlSlot(QString name, ControlContext* context) :
		_instantaneousValue(0),
		_accumulatedValue(0),
		_value(0),
		ControlSlot(name, context)
{
} // end AnalogControlSlot

const ControlSlot::Type AnalogControlSlot::type() const
{
	return ControlSlot::ANALOG;
} // end type

float AnalogControlSlot::instantaneousValue() const
{
	return _instantaneousValue;
} // end instantaneousValue

float AnalogControlSlot::accumulatedValue() const
{
	return _accumulatedValue;
} // end accumulatedValue

float AnalogControlSlot::value() const
{
	return _value;
} // end value

void AnalogControlSlot::onBindingStateChanged()
{
	//FIXME: Implement!
} // end onBindingStateChanged

void AnalogControlSlot::onBindingAttached(ControlBinding* binding)
{
	ControlSlot::onBindingAttached(binding);
} // end onBindingAttached

void AnalogControlSlot::onBindingRemoved(ControlBinding* binding)
{
	ControlSlot::onBindingRemoved(binding);
} // end onBindingRemoved
