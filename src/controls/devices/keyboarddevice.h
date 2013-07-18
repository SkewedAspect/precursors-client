#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include "inputdevice.h"


class KeyboardDevice : public InputDevice
{
    Q_OBJECT

public:
    explicit KeyboardDevice(QObject *parent = 0);
};

#endif // KEYBOARDDEVICE_H
