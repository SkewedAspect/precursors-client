#include "axisinputsignal.h"


AxisInputSignal::AxisInputSignal(InputDevice* device, QString name, QString description) :
		InputSignal(device, name, description)
{
} // end AxisInputSignal

const InputSignal::Type AxisInputSignal::type() const
{
	return InputSignal::AXIS;
} // end type

void AxisInputSignal::emitUpdated(float position)
{
	emit updated(position);
} // end emitUpdated
