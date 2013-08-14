#include "inputsignal.h"


QString InputSignal::typeString(const InputSignal::Type type)
{
	switch(type)
	{
		case AXIS:
			return "axis";
		case BUTTON:
			return "button";
		default:
			return "UNKNOWN";
	} // end switch
} // end typeString


InputSignal::InputSignal(InputDevice* device, QString name, QString description) :
		_name(name),
		_description(description.isNull() ? name : description),
		_device(device),
		QObject((QObject*) device)
{
} // end InputSignal

const QString InputSignal::name() const
{
	return _name;
} // end name

const QString InputSignal::description() const
{
	return _description;
} // end description

InputDevice* InputSignal::device() const
{
	return _device;
} // end device
