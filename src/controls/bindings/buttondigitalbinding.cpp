#include "buttondigitalbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
ButtonDigitalBinding::ButtonDigitalBinding(ControlBindingMap* bindingMap) :
		BaseDigitalBinding(bindingMap),
		_lastPressedState(false)
{
    // Emit stateChanged whenever any of our state properties change.
    connect(this, SIGNAL(isOnChanged()), this, SIGNAL(stateChanged()));
} // end ButtonDigitalBinding

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
	setInputState(pressed);
} // end onSignalUpdated
