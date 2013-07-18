#include "buttonanalogbinding.h"


/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
ButtonAnalogBinding::ButtonAnalogBinding(QObject *parent) :
    ControlBinding(parent),
    _momentaryValue(0),
    _setValue(0),
    _changeRate(0),
    _instantaneousValue(0),
    _accumulatedValue(0),
    _value(0),
    _mode(BM_UNDEFINED)
{
    // Annoyingly, `Q_PROPERTY` doesn't work with signals defined in the base class. So, we cheat.
    connect(this, SIGNAL(stateChanged()), parent, SIGNAL(stateChanged()));
}

/**
 * @brief Returns the current mode for this binding.
 * @return A member of the BindingMode enum.
 */
ButtonAnalogBinding::BindingMode ButtonAnalogBinding::mode()
{
    return _mode;
} // end mode

float ButtonAnalogBinding::getMomentaryValue()
{
    return _momentaryValue;
} // end getMomentary

float ButtonAnalogBinding::getSetValue()
{
    return _setValue;
} // end getSetValue

float ButtonAnalogBinding::getChangeRate()
{
    return _changeRate;
} // end getChangeRate

void ButtonAnalogBinding::setMomentaryValue(float val)
{
    _mode = BM_MOMENTARY;
    _momentaryValue = val;
} // end setMomentaryValue

void ButtonAnalogBinding::setSetValue(float val)
{
    _mode = BM_SETVALUE;
    _setValue = val;
} // end setSetValue

void ButtonAnalogBinding::setChangeRate(float val)
{
    _mode = BM_CHANGERATE;
    _setValue = val;
} // end setChangeRate

/**********************************************************************************************************************/
/* Slots                                                                                                              */
/**********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param pressed True if the button is currently being pressed, false otherwise.
 * @param repeating True if this slot is being called because of a key repeate event.
 */
void ButtonAnalogBinding::onSignalUpdated(bool pressed, bool repeating)
{
    // If the 'invert' option is disabled, this equates to 'if pressed'; if 'invert' is enabled, it equates to
    // 'if not pressed'.
    bool currentState = (pressed == _invert);

    switch (_mode)
    {
        case BM_MOMENTARY:
        {
            _instantaneousValue = currentState ? _momentaryValue : 0;
            emit stateChanged();
            break;
        }

        case BM_CHANGERATE:
        {
            _accumulatedValue = currentState ? _changeRate : 0;
            emit stateChanged();
            break;
        }

        case BM_SETVALUE:
        {
            _value = currentState ? _setValue : 0;
            emit stateChanged();
            break;
        }

        default:
        {
            _instantaneousValue = 0;
            _accumulatedValue = 0;
            _value = 0;
            break;
        } // end default
    } // end switch
}
