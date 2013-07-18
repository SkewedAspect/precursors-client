#ifndef AXISDIGITALBINDING_H
#define AXISDIGITALBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between an axis signal and a digital input slot.

    This binding type takes 3 options:

    - `threshold`: The value over which this binding is considered "active". (or inactive, if `invert` is True)
    - `overlap`: The difference between the triggering threshold and the un-triggering threshold. (to overcome jitter)
    - `invert`: If True, trigger the binding on a "low" value instead of a "high" value.
 */
class AxisDigitalBinding : public ControlBinding
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool invert MEMBER _invert)
    Q_PROPERTY(float threshold MEMBER _threshold)
    Q_PROPERTY(float  overlap MEMBER _overlap)

public:
    explicit AxisDigitalBinding(QObject *parent = 0);

    bool state();

signals:
    void stateChanged();

public slots:
    void onSignalUpdated(float position);

 private:
    float _threshold;
    float _overlap;
    bool _invert;
    bool _state;
};

#endif // AXISDIGITALBINDING_H
