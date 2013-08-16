#include "analogcontrolslot.h"
#include "controls/controlcontext.h"
#include "controls/bindings/buttonanalogbinding.h"


AnalogControlSlot::AnalogControlSlot(QString name, ControlContext* context) :
		_instantaneousValue(0),
		_accumulatedValue(0),
		_value(0),
		_logger(PLogManager::getLogger(QString("AnalogControlSlot[\"%1\" in \"%2\"]").arg(name).arg(context->name()))),
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

void AnalogControlSlot::onBindingMomentaryStateSet()
{
	_instantaneousValue = 0;
	foreach(ControlBinding* binding, bindings())
	{
		ButtonAnalogBinding* buttonAnalog = qobject_cast<ButtonAnalogBinding*>(binding);
		if(buttonAnalog && buttonAnalog->mode() == ButtonAnalogBinding::BM_MOMENTARY && buttonAnalog->isOn())
		{
			_instantaneousValue += buttonAnalog->momentaryValue();
			break;
		} // end if
	} // end foreach

	updateValue();
} // end onBindingMomentaryStateChanged

void AnalogControlSlot::onBindingChangeRateSet()
{
	_accumulatedValue = 9999999999;
	updateValue();
} // end onBindingChangeRateChanged

void AnalogControlSlot::onBindingSetTo(float val)
{
	_accumulatedValue = val;
	updateValue();
} // end onBindingSetTo

void AnalogControlSlot::onBindingAttached(ControlBinding* binding)
{
	ControlSlot::onBindingAttached(binding);
} // end onBindingAttached

void AnalogControlSlot::onBindingRemoved(ControlBinding* binding)
{
	ControlSlot::onBindingRemoved(binding);
} // end onBindingRemoved

void AnalogControlSlot::updateValue()
{
	_value = _instantaneousValue + _accumulatedValue;
    emit valueChanged(_value);
} // end updateValue
