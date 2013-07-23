#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "inputdevice.h"


class GenericDevice : public InputDevice
{
    Q_OBJECT

public:
    explicit GenericDevice(InputDriver* driver, QString id);

    void addAxisSignal(AxisInputSignal* signal);
    void addButtonSignal(ButtonInputSignal* signal);
}; // end GenericDevice

#endif // GENERICDEVICE_H
