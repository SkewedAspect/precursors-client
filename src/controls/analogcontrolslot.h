#ifndef ANALOGCONTROLSLOT_H
#define ANALOGCONTROLSLOT_H

#include "controlslot.h"

class AnalogControlSlot : public ControlSlot
{
    Q_OBJECT
public:
    explicit AnalogControlSlot(QObject *parent = 0);

    float instantaneousValue;
    float accumulatedValue;
    float value;

signals:
    void valueChanged(float value);
};

#endif // ANALOGCONTROLSLOT_H
