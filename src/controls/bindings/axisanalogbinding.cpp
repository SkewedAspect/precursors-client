#include "axisanalogbinding.h"


/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisAnalogBinding::AxisAnalogBinding(QObject *parent) :
    ControlBinding(parent),
    _sensitivity(0),
    _offset(0),
    _deadzone(0),
    _instantaneousValue(0)
{
} // end AxisAnalogBinding

/**********************************************************************************************************************/
/* Slots                                                                                                              */
/**********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param position The position of the axis.
 */
void AxisAnalogBinding::onSignalUpdated(float position)
{
    _instantaneousValue = position;
    emit stateChanged();
} // onSignalUpdated
