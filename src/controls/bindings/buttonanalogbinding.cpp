#include <QVariantMap>

#include "buttonanalogbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
ButtonAnalogBinding::ButtonAnalogBinding(ControlBindingMap* bindingMap) :
		ControlBinding(bindingMap),
		_isOn(false),
		_mode(BM_UNDEFINED),
		_momentaryValue(0),
		_valueToSet(0),
		_incrementValue(0),
		_changeRate(0),
		_invert(false)
{
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

float ButtonAnalogBinding::incrementValue()
{
	return _incrementValue;
} // end incrementValue

float ButtonAnalogBinding::changeRate()
{
    return _changeRate;
} // end changeRate

bool ButtonAnalogBinding::invert()
{
	return _invert;
} // end invert

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

void ButtonAnalogBinding::setIncrementValue(float val)
{
    _incrementValue = val;
    setMode(BM_INCREMENT);
    emit incrementValueChanged();
} // end setIncrementValue

void ButtonAnalogBinding::setChangeRate(float val)
{
    _changeRate = val;
    setMode(BM_CHANGERATE);
    emit changeRateChanged();
} // end setChangeRate

void ButtonAnalogBinding::setInvert(bool invert)
{
	_invert = invert;
} // end setInvert

bool ButtonAnalogBinding::configure(QVariantMap bindingDef)
{
    if(bindingDef.contains("momentaryValue"))
	{
		setMomentaryValue(bindingDef["momentaryValue"].toFloat());
	} // end if

    if(bindingDef.contains("valueToSet"))
	{
		setValueToSet(bindingDef["valueToSet"].toFloat());
	} // end if

    if(bindingDef.contains("incrementValue"))
	{
		setIncrementValue(bindingDef["incrementValue"].toFloat());
	} // end if

    if(bindingDef.contains("changeRate"))
	{
		setChangeRate(bindingDef["changeRate"].toFloat());
	} // end if

	bool newInvert = bindingDef.value("invert", false).toBool();
	if(newInvert != _invert)
	{
		_invert = newInvert;
		emit invertChanged();
	} // end if
} // end configure

/*********************************************************************************************************************/
/* Slots                                                                                                             */
/*********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param pressed true if the button is currently being pressed, false otherwise.
 * @param repeating true if this slot is being called because of a key repeat event.
 */
void ButtonAnalogBinding::onSignalUpdated(bool pressed, bool repeating)
{
	if(repeating)
	{
		return;
	} // end if

    // If the 'invert' option is disabled, this equates to 'if pressed'; if 'invert' is enabled, it equates to
    // 'if not pressed'.
    bool newState = (pressed != _invert);

    if(_isOn != newState)
	{
		_isOn = newState;

		if(isActive())
		{
			switch(_mode)
			{
				case BM_MOMENTARY:
					emit momentaryStateSet();
					break;
				case BM_SETVALUE:
					emit setTo(_valueToSet);
					break;
				case BM_INCREMENT:
					emit increment(_incrementValue);
					break;
				case BM_CHANGERATE:
					emit changeRateSet();
					break;
			} // end switch
		} // end if
	} // end if
} // end onSignalUpdated

/*********************************************************************************************************************/
/* Private Methods                                                                                                   */
/*********************************************************************************************************************/

void ButtonAnalogBinding::setMode(BindingMode newMode)
{
    if(_mode == newMode)
	{
		if(isActive())
		{
			// In this case, although the values of `mode` and `isOn` didn't change, one of the parameters for the
			// current mode did, so we need to tell our slot to update.
			switch(_mode)
			{
				case BM_MOMENTARY:
					emit momentaryStateSet();
					break;
				case BM_SETVALUE:
					emit setTo(_valueToSet);
					break;
				case BM_INCREMENT:
					emit increment(_incrementValue);
					break;
				case BM_CHANGERATE:
					emit changeRateSet();
					break;
			} // end switch
		} // end if
	}
	else
	{
		if(isActive())
		{
			bool wasOn = _isOn;
			_isOn = false;
			switch(_mode)
			{
				case BM_MOMENTARY:
					emit momentaryStateSet();
					break;
				case BM_CHANGERATE:
					emit changeRateSet();
					break;
			} // end switch
			_isOn = wasOn;

			_mode = newMode;
			emit modeChanged();
		}
		else
		{
			_mode = newMode;
		} // end if
	} // end if
} // end setMode
