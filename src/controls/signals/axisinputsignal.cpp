#include "controls/devices/inputdevice.h"

#include "axisinputsignal.h"


AxisInputSignal::AxisInputSignal(InputDevice* device, QString name, QString description) :
		_settings(PSettingsManager::instance()),
		InputSignal(device, name, description)
{
	QVariantMap calibrationConfig = calibration();
	_centerValue = calibrationConfig.value("centerValue", 0).toDouble();
	_deadZone = calibrationConfig.value("deadZone", 0).toDouble();
	_maxValue = calibrationConfig.value("maxValue", 1).toDouble();
	_minValue = calibrationConfig.value("minValue", -1).toDouble();
} // end AxisInputSignal

const InputSignal::Type AxisInputSignal::type() const
{
	return InputSignal::AXIS;
} // end type

float AxisInputSignal::centerValue() const
{
	return _centerValue;
} // end centerValue

float AxisInputSignal::deadZone() const
{
	return _deadZone;
} // end deadZone

float AxisInputSignal::maxValue() const
{
	return _maxValue;
} // end maxValue

float AxisInputSignal::minValue() const
{
	return _minValue;
} // end minValue

QVariantMap AxisInputSignal::calibration() const
{
	return device()->config()["calibration"]
			.toMap()[name()]
			.toMap();
} // end calibration

void AxisInputSignal::setCenterValue(float value)
{
	_centerValue = value;
} // end setCenterValue

void AxisInputSignal::setDeadZone(float value)
{
	_deadZone = value;
} // end setDeadZone

void AxisInputSignal::setMaxValue(float value)
{
	_maxValue = value;
} // end setMaxValue

void AxisInputSignal::setMinValue(float value)
{
	_minValue = value;
} // end setMinValue

void AxisInputSignal::emitUpdated(float position)
{
	position -= _centerValue;

	float max = _maxValue;

	if(position < 0)
	{
		position *= -1;
		max = _minValue;
	} // end if

	if(position < _deadZone)
	{
		position = 0;
	}
	else
	{
		position -= _deadZone;
		position /= max;
	} // end if

	emit updated(position);
} // end emitUpdated
