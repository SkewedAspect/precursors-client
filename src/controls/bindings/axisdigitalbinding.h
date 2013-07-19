#ifndef AXISDIGITALBINDING_H
#define AXISDIGITALBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between an axis signal and a digital input slot.
 *
 * This binding type takes 3 options:
 *
 * - `threshold`: The value over which this binding is considered "active". (or inactive, if `invert` is true)
 * - `overlap`: The difference between the triggering threshold and the un-triggering threshold. (to overcome jitter)
 * - `invert`: If true, trigger the binding on a "low" value instead of a "high" value.
 */
class AxisDigitalBinding : public ControlBinding
{
    Q_OBJECT

    Q_PROPERTY(bool isOn READ isOn NOTIFY isOnChanged)

    Q_PROPERTY(float threshold MEMBER _threshold)
    Q_PROPERTY(float overlap MEMBER _overlap)
    Q_PROPERTY(bool invert MEMBER _invert)

public:
    explicit AxisDigitalBinding(QObject *parent = 0);

    bool isOn();

signals:
    void isOnChanged();

public slots:
    void onSignalUpdated(float position);

 private:
    bool _isOn;

    float _threshold;
    float _overlap;
    bool _invert;
}; // end AxisDigitalBinding

#endif // AXISDIGITALBINDING_H
