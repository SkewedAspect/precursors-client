#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "inputdevice.h"


class GenericDevice : public InputDevice
{
    Q_OBJECT

public:
    explicit GenericDevice(InputDriver* driver, InputDevice::Type type, QString id);

	virtual const InputDevice::Type type() const;

    void addAxisSignal(AxisInputSignal* signal);
    void addButtonSignal(ButtonInputSignal* signal);

protected:
	virtual PLogger& _logger() const;

private:
	InputDevice::Type _type;
}; // end GenericDevice

#endif // GENERICDEVICE_H
