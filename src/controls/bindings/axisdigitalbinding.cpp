#include "axisdigitalbinding.h"


/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisDigitalBinding::AxisDigitalBinding(QObject *parent) :
    ControlBinding(parent),
    _threshold(0),
    _overlap(0),
    _invert(false),
    _state(false)
{
    // Annoyingly, `Q_PROPERTY` doesn't work with signals defined in the base class. So, we cheat.
    connect(this, SIGNAL(stateChanged()), parent, SIGNAL(stateChanged()));
} // end AxisDigitlBinding

bool AxisDigitalBinding::state()
{
   return this->_state;
} // end state

/**********************************************************************************************************************/
/* Slots                                                                                                              */
/**********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param position The position of the axis.
 */
void AxisDigitalBinding::onSignalUpdated(float position)
{
    bool previousState = _state;

    if(previousState != _invert)
    {
         // If either we were previously active and `invert` is False, or if we were inactive and `invert` is True,
         // then change if we fall below `threshold` - `overlap`.
        if(position < (_threshold - _overlap))
        {
            _state = !previousState;
            emit stateChanged();
        } // end if
    }
    else
    {
        // Otherwise, change if we go above `threshold`.
        if(position > _threshold)
        {
            _state = !previousState;
            emit stateChanged();
        } // end if
    } // end if
} // end onSignalUpdated
