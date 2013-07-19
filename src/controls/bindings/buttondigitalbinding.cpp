#include "buttondigitalbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

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
    // Annoyingly, `Q_PROPERTY` doesn't work with signals defined in the base class. So, we cheat.
    connect(this, SIGNAL(stateChanged()), this, SIGNAL(ControlBinding::stateChanged()));
} // end ButtonDigitalBinding

/**
 * @brief Returns the current state of this binding.
 * @return true, if this binding is "on", and false if it is "off".
 */
bool ButtonDigitalBinding::state()
{
    return _state;
} // end state

/*********************************************************************************************************************/
/* Slots                                                                                                             */
/*********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param pressed true if the button is currently being pressed, false otherwise.
 * @param repeating true if this slot is being called because of a key repeate event.
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
