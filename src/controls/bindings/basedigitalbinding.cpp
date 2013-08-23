#include <QVariantMap>

#include "basedigitalbinding.h"


/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief Default Constructor
 * @param parent The parent QObject.
 */
BaseDigitalBinding::BaseDigitalBinding(ControlBindingMap* bindingMap) :
		ControlBinding(bindingMap),
		_isOn(false),
		_invert(false),
		_toggle(false),
		_lastInputState(false)
{
    // Emit stateChanged whenever any of our state properties change.
    connect(this, SIGNAL(isOnChanged()), this, SIGNAL(stateChanged()));
} // end BaseDigitalBinding

/**
 * @brief Returns the current isOn of this binding.
 * @return true, if this binding is "on", and false if it is "off".
 */
bool BaseDigitalBinding::isOn()
{
    return _isOn;
} // end isOn

bool BaseDigitalBinding::invert()
{
	return _invert;
} // end invert

bool BaseDigitalBinding::toggle()
{
	return _toggle;
} // end toggle

void BaseDigitalBinding::setInvert(bool invert)
{
	_invert = invert;
} // end setInvert

void BaseDigitalBinding::setToggle(bool toggle)
{
	_toggle = toggle;
} // end setToggle

bool BaseDigitalBinding::configure(QVariantMap bindingDef)
{
	_invert = bindingDef.value("invert", false).toBool();
	_toggle = bindingDef.value("toggle", false).toBool();

    emit invertChanged();
    emit toggleChanged();
} // end configure

bool BaseDigitalBinding::inputState()
{
	return _lastInputState;
} // end inputState

void BaseDigitalBinding::setInputState(bool state)
{
    if(_lastInputState == state)
	{
		return;
	} // end if

	_isOn = (state != _invert);

    if(_toggle)
    {
        // Only toggle if our last state was off, and our current state is on. (or vice versa if 'invert' is enabled)
		// If !invert, detect off => on transition (state == true); if invert, detect on => off (state == false)
        if(_isOn && isActive())
        {
            emit triggered();
        } // end if
	}
	else if(isActive())
	{
		emit isOnChanged();
    } // end if

    _lastInputState = state;
} // end setInputState
