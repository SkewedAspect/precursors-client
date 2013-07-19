#ifndef BUTTONANALOGBINDING_H
#define BUTTONANALOGBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between a button signal and an analog input slot.
 *
 * This has 3 different modes, each with their own possible options:
 *
 * - momentary: While the button is pressed (or not pressed, if `invert` is true), add the value given by
 *     `momentaryValue` to the total for this slot.
 * - set to: When the button is pressed, this slot's accumulated value is set to the value given by `setValue`.
 * - rate of change: While the button is pressed, change the slot's accumulated value by `changeRate` every second.
 */
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
    /**
     * @brief Represents the mode of a ButtonAnalogBinding instance.
     */
    enum BindingMode
    {
        BM_UNDEFINED,	/*!< No mode has been set yet. This is the initial mode. */
        BM_MOMENTARY,	/*!< Momentary mode. */
        BM_SETVALUE,	/*!< Set To mode. */
        BM_CHANGERATE	/*!< Change Rate mode. */
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
