#include <QVariantMap>

#include "axisdigitalbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
AxisDigitalBinding::AxisDigitalBinding(ControlBindingMap* bindingMap) :
		BaseDigitalBinding(bindingMap),
		_threshold(0),
		_overlap(0),
		_lowerThreshold(0),
		_upperThreshold(0)
{
} // end AxisDigitalBinding

float AxisDigitalBinding::threshold()
{
	return _threshold;
} // end threshold

float AxisDigitalBinding::overlap()
{
	return _overlap;
} // end overlap

float AxisDigitalBinding::lowerThreshold()
{
	return _lowerThreshold;
} // end lowerThreshold

float AxisDigitalBinding::upperThreshold()
{
	return _upperThreshold;
} // end upperThreshold

void AxisDigitalBinding::setThreshold(float threshold)
{
	_threshold = threshold;
	updateTriggerRange();
	emit thresholdChanged();
} // end setThreshold

void AxisDigitalBinding::setOverlap(float overlap)
{
	_overlap = overlap;
	updateTriggerRange();
	emit overlapChanged();
} // end setOverlap

bool AxisDigitalBinding::configure(QVariantMap bindingDef)
{
	BaseDigitalBinding::configure(bindingDef);

    if(bindingDef.contains("threshold"))
	{
		setThreshold(bindingDef["threshold"].toFloat());
	} // end if

    if(bindingDef.contains("overlap"))
	{
		setOverlap(bindingDef["overlap"].toFloat());
	} // end if
} // end configure

/*********************************************************************************************************************/
/* Slots                                                                                                             */
/*********************************************************************************************************************/

/**
 * @brief Should get called whenever the connected InputSignal's value changes.
 * @param position The position of the axis.
 */
void AxisDigitalBinding::onSignalUpdated(float position)
{
    if(inputState())
    {
         // If the input state was previously on, then change to off if we fall below `_lowerThreshold`.
        if(position < _lowerThreshold)
        {
			setInputState(false);
        } // end if
    }
    else
    {
        // Otherwise, change if we go above `_upperThreshold`.
        if(position > _upperThreshold)
        {
			setInputState(true);
        } // end if
    } // end if
} // end onSignalUpdated

void AxisDigitalBinding::updateTriggerRange()
{
	_lowerThreshold = _threshold - (_overlap / 2);
	_upperThreshold = _threshold + (_overlap / 2);
	emit triggerRangeChanged();
} // end updateTriggerRange
