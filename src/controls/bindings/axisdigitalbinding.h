#ifndef AXISDIGITALBINDING_H
#define AXISDIGITALBINDING_H

#include "controlbinding.h"


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
