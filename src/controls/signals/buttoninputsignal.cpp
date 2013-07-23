#include "buttoninputsignal.h"


ButtonInputSignal::ButtonInputSignal(InputDevice* device, QString name, QString description) :
		InputSignal(device, name, description)
{
} // end ButtonInputSignal

void ButtonInputSignal::emitUpdated(bool pressed, bool repeating)
{
	emit updated(pressed, repeating);
} // end emitUpdated
