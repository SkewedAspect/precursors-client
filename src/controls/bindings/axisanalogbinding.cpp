#include "axisanalogbinding.h"


// Assume the range of values from the axis is approximately in a 16-bit int range
#define DEFAULT_SENSITIVITY 1.f / 32768.f


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisAnalogBinding::AxisAnalogBinding(ControlBindingMap* bindingMap) :
		_value(0),
		_sensitivity(DEFAULT_SENSITIVITY),
		_offset(0),
		_deadZone(0),
		_logger(PLogManager::getLogger("AxisAnalogBinding")),
		ControlBinding(bindingMap)
{
} // end AxisAnalogBinding

float AxisAnalogBinding::value()
{
	return _value;
} // end value

bool AxisAnalogBinding::configure(QVariantMap bindingDef)
{
    _sensitivity = bindingDef.value("sensitivity", DEFAULT_SENSITIVITY).toFloat();
    _offset = bindingDef.value("offset", 0.f).toFloat();
    _deadZone = bindingDef.value("deadZone", 0.f).toFloat();

    emit sensitivityChanged();
    emit offsetChanged();
    emit deadZoneChanged();
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
		emit setTo(_value);
    } // end if
} // onSignalUpdated
