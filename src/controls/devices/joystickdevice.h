#ifndef JOYSTICKDEVICE_H
#define JOYSTICKDEVICE_H

#include "inputdevice.h"


class JoystickDevice : public InputDevice
{
    Q_OBJECT

public:
    explicit JoystickDevice(QObject *parent = 0);
};

#endif // JOYSTICKDEVICE_H
