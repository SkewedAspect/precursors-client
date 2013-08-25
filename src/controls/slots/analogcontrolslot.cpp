#include "analogcontrolslot.h"
#include "controls/controlcontext.h"
#include "controls/bindings/buttonanalogbinding.h"
#include "controls/bindings/axisanalogbinding.h"


AnalogControlSlot::AnalogControlSlot(QString name, ControlContext* context) :
		_instantaneousValue(0),
		_accumulatedValue(0),
		_accumulatedRateOfChange(0),
		_value(0),
		_timerRunning(false),
		_logger(PLogManager::getLogger(QString("AnalogControlSlot[\"%1\" in \"%2\"]").arg(name).arg(context->name()))),
		ControlSlot(name, context)
{
	_timeSinceLastROC.start();
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

void AnalogControlSlot::onBindingSetTo(float val)
{
	_accumulatedValue = val;
	updateValue();
} // end onBindingSetTo

void AnalogControlSlot::onBindingIncrement(float val)
{
	_accumulatedValue += val;
	updateValue();
} // end onBindingIncrement

void AnalogControlSlot::onBindingChangeRateSet()
{
	int elapsedMS = _timeSinceLastROC.restart();
	if(_accumulatedRateOfChange != 0)
	{
		_accumulatedValue += _accumulatedRateOfChange * elapsedMS / 1000;
	} // end if

	_accumulatedRateOfChange = 0;
	foreach(ControlBinding* binding, bindings())
	{
		ButtonAnalogBinding* buttonAnalog = qobject_cast<ButtonAnalogBinding*>(binding);
		if(buttonAnalog && buttonAnalog->mode() == ButtonAnalogBinding::BM_CHANGERATE && buttonAnalog->isOn())
		{
			_accumulatedRateOfChange += buttonAnalog->changeRate();
		}
		else
		{
			AxisAnalogBinding* axisAnalog = qobject_cast<AxisAnalogBinding*>(binding);
			if(axisAnalog && axisAnalog->isDelta())
			{
				_accumulatedRateOfChange += axisAnalog->value();
			} // end if
		} // end if
	} // end foreach

	if(_accumulatedRateOfChange == 0 && _timerRunning)
	{
		killTimer(_timerID);
		_timerRunning = false;
	}
	else if(_accumulatedRateOfChange != 0 && !_timerRunning)
	{
		_timerRunning = true;
		_timerID = startTimer(30);
	} // end if

	updateValue();
} // end onBindingChangeRateChanged

void AnalogControlSlot::onBindingAttached(ControlBinding* binding)
{
	ControlSlot::onBindingAttached(binding);
} // end onBindingAttached

void AnalogControlSlot::onBindingRemoved(ControlBinding* binding)
{
	ControlSlot::onBindingRemoved(binding);
} // end onBindingRemoved

void AnalogControlSlot::timerEvent(QTimerEvent* event)
{
	int elapsedMS = _timeSinceLastROC.restart();

	_accumulatedValue += _accumulatedRateOfChange * elapsedMS / 1000;

	updateValue();
} // end timerEvent

void AnalogControlSlot::updateValue()
{
	_value = _instantaneousValue + _accumulatedValue;
    emit valueChanged(_value);
} // end updateValue
