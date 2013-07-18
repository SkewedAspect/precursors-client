#ifndef BUTTONANALOGBINDING_H
#define BUTTONANALOGBINDING_H

#include "controlbinding.h"


class ButtonAnalogBinding : public ControlBinding
{
    Q_OBJECT

    Q_PROPERTY(bool momentaryValue MEMBER _momentaryValue)
    Q_PROPERTY(bool setValue MEMBER _setValue)
    Q_PROPERTY(bool changeRate MEMBER _changeRate)
    Q_PROPERTY(bool invert MEMBER _invert)

    Q_PROPERTY(bool instantaneousValue MEMBER _instantaneousValue)
    Q_PROPERTY(bool accumulatedValue MEMBER _accumulatedValue)
    Q_PROPERTY(bool value MEMBER _value)

public:
    explicit ButtonAnalogBinding(QObject *parent = 0);

    bool isMomentary();

public slots:
    void onSignalUpdated(bool pressed, bool repeating);

private:
    float _momentaryValue;
    float _setValue;
    float _changeRate;
    bool _invert;

    float _instantaneousValue;
    float _accumulatedValue;
    float _value;


};

#endif // BUTTONANALOGBINDING_H
