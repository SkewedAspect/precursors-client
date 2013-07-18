#ifndef BUTTONANALOGBINDING_H
#define BUTTONANALOGBINDING_H

#include "controlbinding.h"


class ButtonAnalogBinding : public ControlBinding
{
    Q_OBJECT
    Q_ENUMS(BindingMode)

    Q_PROPERTY(float momentaryValue READ getMomentaryValue WRITE setMomentaryValue NOTIFY momentaryValueChanged)
    Q_PROPERTY(float setValue READ getSetValue WRITE setSetValue NOTIFY setValueChanged)
    Q_PROPERTY(float changeRate READ getChangeRate WRITE setChangeRate NOTIFY changeRateChanged)

    Q_PROPERTY(BindingMode mode READ mode NOTIFY modeChanged);
    Q_PROPERTY(bool invert MEMBER _invert)

    Q_PROPERTY(float instantaneousValue MEMBER _instantaneousValue)
    Q_PROPERTY(float accumulatedValue MEMBER _accumulatedValue)
    Q_PROPERTY(float value MEMBER _value)

public:
    enum BindingMode
    {
        BM_UNDEFINED,
        BM_MOMENTARY,
        BM_SETVALUE,
        BM_CHANGERATE
    };

    explicit ButtonAnalogBinding(QObject *parent = 0);

    BindingMode mode();

    float getMomentaryValue();
    float getSetValue();
    float getChangeRate();

    void setMomentaryValue(float val);
    void setSetValue(float val);
    void setChangeRate(float val);

signals:
    void momentaryValueChanged();
    void setValueChanged();
    void changeRateChanged();
    void modeChanged();

public slots:
    void onSignalUpdated(bool pressed, bool repeating);

private:
    float _momentaryValue;
    float _setValue;
    float _changeRate;

    bool _invert;
    BindingMode _mode;

    float _instantaneousValue;
    float _accumulatedValue;
    float _value;
};

#endif // BUTTONANALOGBINDING_H
