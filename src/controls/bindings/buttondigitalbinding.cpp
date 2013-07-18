#include "buttondigitalbinding.h"

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

ButtonDigitalBinding::ButtonDigitalBinding(QObject *parent) :
    ControlBinding(parent),
    _invert(false),
    _toggle(false),
    _state(false)
{
} // end ButtonDigitalBinding

bool ButtonDigitalBinding::state()
{
    return this->_state;
} // end state

/**********************************************************************************************************************/
/* Slots                                                                                                              */
/**********************************************************************************************************************/

void ButtonDigitalBinding::onSignalUpdated(bool pressed, bool repeating)
{
    if(_toggle)
    {
        // Only toggle if our last state was not pressed, and our current state is pressed.
        if(!_lastPressedState && pressed)
        {
            emit stateChanged();
            _state = !_state;
        } // end if
    }
    else
    {
        _state = (pressed != _invert);
    } // end if

    _lastPressedState = pressed;
} // end onSignalUpdated
