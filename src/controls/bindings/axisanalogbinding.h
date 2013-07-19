#ifndef AXISANALOGBINDING_H
#define AXISANALOGBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between an axis signal and an analog input slot.
 */
class AxisAnalogBinding : public ControlBinding
{
    Q_OBJECT

    Q_PROPERTY(float value READ value NOTIFY valueChanged)

    Q_PROPERTY(float sensitivity MEMBER _sensitivity NOTIFY sensitivityChanged)
    Q_PROPERTY(float offset MEMBER _offset NOTIFY offsetChanged)
    Q_PROPERTY(float deadZone MEMBER _deadZone NOTIFY deadZoneChanged)

public:
    explicit AxisAnalogBinding(QObject *parent = 0);

	float value();

signals:
    void valueChanged();

    void sensitivityChanged();
    void offsetChanged();
    void deadZoneChanged();

public slots:
    void onSignalUpdated(float position);

private:
    float _value;

    float _sensitivity;
    float _offset;
    float _deadZone;
}; // end AxisAnalogBinding

#endif // AXISANALOGBINDING_H
