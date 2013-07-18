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
    _value(0)
{
}

/**
 * @brief Returns whether or not this slot is momentary.
 * @return True if the slot is momentary, otherwise false.
 */
bool ButtonAnalogBinding::isMomentary()
{
    if(_momentaryValue !=0)
    {
        return true;
    } // end if

    return false;
} // end ButtonAnalogBindings

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

    if(isMomentary())
    {
       _instantaneousValue = currentState ? _momentaryValue : 0;
    }
    else if(_changeRate != 0)
    {
        _accumulatedValue = currentState ? _changeRate : 0;
    }
    else if(_setValue != 0)
    {
        _value = currentState ? _setValue : 0;
    } // end if
} // end onSignalUpdated
