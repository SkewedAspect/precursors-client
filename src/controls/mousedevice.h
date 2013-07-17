#ifndef MOUSEDEVICE_H
#define MOUSEDEVICE_H

#include "inputdevice.h"

class MouseDevice : public InputDevice
{
    Q_OBJECT
public:
    explicit MouseDevice(QObject *parent = 0);
};

#endif // MOUSEDEVICE_H
