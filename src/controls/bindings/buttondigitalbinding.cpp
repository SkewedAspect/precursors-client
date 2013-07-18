#include "buttondigitalbinding.h"


/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
ButtonDigitalBinding::ButtonDigitalBinding(QObject *parent) :
    ControlBinding(parent),
    _invert(false),
    _toggle(false),
    _state(false)
{
    connect(this, SIGNAL(stateChanged()), parent, SIGNAL(stateChanged()));
} // end ButtonDigitalBinding

/**
 * @brief Returns the current state of this binding.
 * @return True, if this binding is "on", and false if it is "off".
 */
bool ButtonDigitalBinding::state()
{
    return this->_state;
} // end state

/**********************************************************************************************************************/
/* Slots                                                                                                              */
/**********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param pressed True if the button is currently being pressed, false otherwise.
 * @param repeating True if this slot is being called because of a key repeate event.
 */
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
        emit stateChanged();
        _state = (pressed != _invert);
    } // end if

    _lastPressedState = pressed;
} // end onSignalUpdated
