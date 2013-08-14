#ifndef BUTTONDIGITALBINDING_H
#define BUTTONDIGITALBINDING_H

#include "basedigitalbinding.h"


/**
 * @brief A binding between a button signal and a digital input slot.
 *
 * This binding type takes 2 options:
 *
 * - `toggle`: If true, invert the slot's value when we transition from a "low" value to a "high" value.
 * - `invert`: If true, trigger the binding on a "low" value (or "high" to "low" transition) instead of a "high" value
 *     (or "low" to "high" transition).
 */
class ButtonDigitalBinding : public BaseDigitalBinding
{
    Q_OBJECT

public:
    explicit ButtonDigitalBinding(ControlBindingMap* bindingMap);

public slots:
    void onSignalUpdated(bool pressed, bool repeating);

private:
    bool _lastPressedState;
}; // end ButtonDigitalBinding

#endif // BUTTONDIGITALBINDING_H
