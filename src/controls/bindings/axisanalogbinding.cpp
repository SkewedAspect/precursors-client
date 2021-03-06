#include "axisanalogbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisAnalogBinding::AxisAnalogBinding(ControlBindingMap* bindingMap) :
		_value(0),
		_sensitivity(1),
		_offset(0),
		_deadZone(0),
		_isDelta(false),
		_logger(PLogManager::getLogger("AxisAnalogBinding")),
		ControlBinding(bindingMap)
{
} // end AxisAnalogBinding

float AxisAnalogBinding::value()
{
	return _value;
} // end value

bool AxisAnalogBinding::isDelta()
{
	return _isDelta;
} // end isDelta

void AxisAnalogBinding::setDelta(bool isDelta)
{
	_isDelta = isDelta;
} // end setDelta

bool AxisAnalogBinding::configure(QVariantMap bindingDef)
{
    _sensitivity = bindingDef.value("sensitivity", 1).toFloat();
    _offset = bindingDef.value("offset", 0.f).toFloat();
    _deadZone = bindingDef.value("deadZone", 0.f).toFloat();
	_isDelta = bindingDef.value("delta", false).toBool();

    emit sensitivityChanged();
    emit offsetChanged();
    emit deadZoneChanged();
    emit deltaChanged();

	//TODO: Actually check for errors
	return true;
} // end configure

/*********************************************************************************************************************/
/* Slots                                                                                                             */
/*********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param position The position of the axis.
 */
void AxisAnalogBinding::onSignalUpdated(float position)
{
    float value = position + _offset;
    if(value > 0)
    {
        if(value < _deadZone)
        {
            value = 0;
        }
        else
        {
            value -= _deadZone;
        } // end if
    }
    else
    {
        if(value > -_deadZone)
        {
            value = 0;
        }
        else
        {
            value += _deadZone;
        } // end if
    } // end if

	value *= _sensitivity;

    if(_value != value)
	{
		_value = value;
		if(isActive())
		{
			if(_isDelta)
			{
				emit changeRateSet();
			}
			else
			{
				emit setTo(_value);
			} // end if
		} // end if
    } // end if
} // onSignalUpdated
