#ifndef AXISANALOGBINDING_H
#define AXISANALOGBINDING_H

#include "controlbinding.h"

class AxisAnalogBinding : public ControlBinding
{
    Q_OBJECT
public:
    explicit AxisAnalogBinding(QObject *parent = 0);

    float sensitivity;
    float offset;
    float deadzone;

public slots:
    void onSignalUpdated(float position);
};

#endif // AXISANALOGBINDING_H
