#include "inputsignal.h"


InputSignal::InputSignal(InputDevice* device, QString name, QString description) :
		_name(name),
		_description(description),
		_device(device),
		QObject((QObject*) device)
{
} // end InputSignal

QString InputSignal::name()
{
	return _name;
} // end name

QString InputSignal::description()
{
	return _description;
} // end description

InputDevice* InputSignal::device()
{
	return _device;
} // end device
