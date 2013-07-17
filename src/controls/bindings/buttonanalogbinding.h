#ifndef BUTTONANALOGBINDING_H
#define BUTTONANALOGBINDING_H

#include "controlbinding.h"

class ButtonAnalogBinding : public ControlBinding
{
    Q_OBJECT
public:
    explicit ButtonAnalogBinding(QObject *parent = 0);

    float momentaryValue;
    float setValue;
    float changeRate;

    float instantaneousValue;
    float accumulatedValue;
    float value;

public slots:
    void onSignalUpdated(bool pressed, bool repeating);
};

#endif // BUTTONANALOGBINDING_H
