#include "axisinputsignal.h"


AxisInputSignal::AxisInputSignal(InputDevice* device, QString name, QString description) :
		InputSignal(device, name, description)
{
} // end AxisInputSignal

void AxisInputSignal::emitUpdated(float position)
{
	emit updated(position);
} // end emitUpdated
