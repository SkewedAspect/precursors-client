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
		_isOn(false)
{
    // Emit stateChanged whenever any of our state properties change.
    connect(this, SIGNAL(isOnChanged()), this, SIGNAL(stateChanged()));
} // end ButtonDigitalBinding

/**
 * @brief Returns the current isOn of this binding.
 * @return true, if this binding is "on", and false if it is "off".
 */
bool ButtonDigitalBinding::isOn()
{
    return _isOn;
} // end isOn

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
            emit isOnChanged();
            _isOn = !_isOn;
        } // end if
    }
    else
    {
        emit isOnChanged();
        _isOn = (pressed != _invert);
    } // end if

    _lastPressedState = pressed;
} // end onSignalUpdated
