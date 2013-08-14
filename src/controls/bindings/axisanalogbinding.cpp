#include "axisanalogbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisAnalogBinding::AxisAnalogBinding(ControlBindingMap* bindingMap) :
		ControlBinding(bindingMap),
		_value(0),
		_sensitivity(0),
		_offset(0),
		_deadZone(0)
{
    // Emit stateChanged whenever any of our state properties change.
    connect(this, SIGNAL(valueChanged()), this, SIGNAL(stateChanged()));
} // end AxisAnalogBinding

float AxisAnalogBinding::value()
{
	return _value;
} // end value

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
		emit valueChanged();
    } // end if
} // onSignalUpdated
