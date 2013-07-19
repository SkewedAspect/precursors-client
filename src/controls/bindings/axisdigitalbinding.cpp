#include "axisdigitalbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisDigitalBinding::AxisDigitalBinding(QObject *parent) :
		ControlBinding(parent),
		_isOn(false),
		_threshold(0),
		_overlap(0),
		_invert(false)
{
    // Emit stateChanged whenever any of our state properties change.
    connect(this, SIGNAL(isOnChanged()), this, SIGNAL(stateChanged()));
} // end AxisDigitalBinding

bool AxisDigitalBinding::isOn()
{
   return _isOn;
} // end isOn

/*********************************************************************************************************************/
/* Slots                                                                                                             */
/*********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param position The position of the axis.
 */
void AxisDigitalBinding::onSignalUpdated(float position)
{
    bool previousState = _isOn;

    if(previousState != _invert)
    {
         // If either we were previously active and `invert` is false, or if we were inactive and `invert` is true,
         // then change if we fall below `threshold` - `overlap`.
        if(position < (_threshold - _overlap))
        {
            _isOn = !previousState;
            emit isOnChanged();
        } // end if
    }
    else
    {
        // Otherwise, change if we go above `threshold`.
        if(position > _threshold)
        {
            _isOn = !previousState;
            emit isOnChanged();
        } // end if
    } // end if
} // end onSignalUpdated
