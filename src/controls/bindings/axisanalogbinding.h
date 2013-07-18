#ifndef AXISANALOGBINDING_H
#define AXISANALOGBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between an axis signal and an analog input slot.
 */
class AxisAnalogBinding : public ControlBinding
{
    Q_OBJECT
    Q_PROPERTY(float sensitivity MEMBER _sensitivity)
    Q_PROPERTY(float offset MEMBER _offset)
    Q_PROPERTY(float deadzone MEMBER _deadzone)
    Q_PROPERTY(float instantaneousValue MEMBER _instantaneousValue)

public:
    explicit AxisAnalogBinding(QObject *parent = 0);

public slots:
    void onSignalUpdated(float position);

private:
    float _sensitivity;
    float _offset;
    float _deadzone;

    float _instantaneousValue;
};

#endif // AXISANALOGBINDING_H
