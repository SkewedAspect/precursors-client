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
 * - set to: When the button is pressed, this slot's accumulated value is set to the value given by `valueToSet`.
 * - rate of change: While the button is pressed, change the slot's accumulated value by `changeRate` every second.
 */
class ButtonAnalogBinding : public ControlBinding
{
    Q_OBJECT
    Q_ENUMS(BindingMode)

    Q_PROPERTY(bool isOn READ isOn NOTIFY isOnChanged)

    Q_PROPERTY(float momentaryValue READ momentaryValue WRITE setMomentaryValue NOTIFY momentaryValueChanged)
    Q_PROPERTY(float valueToSet READ valueToSet WRITE setValueToSet NOTIFY valueToSetChanged)
    Q_PROPERTY(float changeRate READ changeRate WRITE setChangeRate NOTIFY changeRateChanged)
    Q_PROPERTY(bool invert MEMBER _invert NOTIFY invertChanged)

    Q_PROPERTY(BindingMode mode READ mode NOTIFY modeChanged);

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

    explicit ButtonAnalogBinding(ControlBindingMap* bindingMap);

    bool isOn();
    BindingMode mode();

    float momentaryValue();
    float valueToSet();
    float changeRate();

    void setMomentaryValue(float val);
    void setValueToSet(float val);
    void setChangeRate(float val);

signals:
    void isOnChanged();

    void momentaryValueChanged();
    void valueToSetChanged();
    void changeRateChanged();
    void invertChanged();

    void modeChanged();

public slots:
    void onSignalUpdated(bool pressed, bool repeating);

private:
    void setMode(BindingMode newMode);

    bool _isOn;

    float _momentaryValue;
    float _valueToSet;
    float _changeRate;
    bool _invert;

    BindingMode _mode;
}; // end ButtonAnalogBinding

#endif // BUTTONANALOGBINDING_H
