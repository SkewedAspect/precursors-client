#include "buttonanalogbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
ButtonAnalogBinding::ButtonAnalogBinding(QObject *parent) :
		ControlBinding(parent),
		_isOn(false),
		_mode(BM_UNDEFINED),
		_momentaryValue(0),
		_valueToSet(0),
		_changeRate(0),
		_invert(false)
{
    // Emit stateChanged whenever any of our state properties change.
    connect(this, SIGNAL(isOnChanged()), this, SIGNAL(stateChanged()));
    connect(this, SIGNAL(invertChanged()), this, SIGNAL(stateChanged()));
} // end ButtonAnalogBinding

/**
 * @brief Returns the current mode for this binding.
 * @return A member of the BindingMode enum.
 */
ButtonAnalogBinding::BindingMode ButtonAnalogBinding::mode()
{
    return _mode;
} // end mode

float ButtonAnalogBinding::momentaryValue()
{
    return _momentaryValue;
} // end momentary

float ButtonAnalogBinding::valueToSet()
{
    return _valueToSet;
} // end valueToSet

float ButtonAnalogBinding::changeRate()
{
    return _changeRate;
} // end changeRate

bool ButtonAnalogBinding::isOn()
{
	return _isOn;
} // end isOn

void ButtonAnalogBinding::setMomentaryValue(float val)
{
    _momentaryValue = val;
    setMode(BM_MOMENTARY);
    emit momentaryValueChanged();
} // end setMomentaryValue

void ButtonAnalogBinding::setValueToSet(float val)
{
    _valueToSet = val;
    setMode(BM_SETVALUE);
    emit valueToSetChanged();
} // end setValueToSet

void ButtonAnalogBinding::setChangeRate(float val)
{
    _valueToSet = val;
    setMode(BM_CHANGERATE);
    emit changeRateChanged();
} // end setChangeRate

/*********************************************************************************************************************/
/* Slots                                                                                                             */
/*********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param pressed true if the button is currently being pressed, false otherwise.
 * @param repeating true if this slot is being called because of a key repeate event.
 */
void ButtonAnalogBinding::onSignalUpdated(bool pressed, bool repeating)
{
    // If the 'invert' option is disabled, this equates to 'if pressed'; if 'invert' is enabled, it equates to
    // 'if not pressed'.
    bool newState = (pressed == _invert);

    if(_isOn != newState)
	{
		_isOn = newState;

		// Only emit isOnChanged if we have a mode set; otherwise, we're effectively disabled.
		switch(_mode)
		{
			case BM_MOMENTARY:
			case BM_CHANGERATE:
			case BM_SETVALUE:
				emit isOnChanged();
				break;
		} // end switch
	} // end if
} // end onSignalUpdated

/*********************************************************************************************************************/
/* Private Methods                                                                                                   */
/*********************************************************************************************************************/

void ButtonAnalogBinding::setMode(BindingMode newMode)
{
    if(_mode == newMode)
	{
		// In this case, although the values of `mode` and `isOn` didn't change, one of the parameters for the current
		// mode did, so we need to tell our slot to update.
		emit stateChanged();
	}
	else
	{
		_mode = newMode;
		emit modeChanged();
	} // end if
} // end setMode
