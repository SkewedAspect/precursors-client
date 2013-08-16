#include "digitalcontrolslot.h"
#include "controls/controlcontext.h"
#include "controls/bindings/basedigitalbinding.h"


DigitalControlSlot::DigitalControlSlot(QString name, ControlContext* context) :
		_toggleState(false),
		_momentaryState(false),
		_logger(PLogManager::getLogger(QString("DigitalControlSlot[%1 in %2]").arg(name).arg(context->name()))),
		ControlSlot(name, context)
{
} // end DigitalControlSlot

const ControlSlot::Type DigitalControlSlot::type() const
{
	return ControlSlot::DIGITAL;
} // end type

bool DigitalControlSlot::state() const
{
	return _toggleState != _momentaryState;
} // end state

void DigitalControlSlot::onToggleBindingTriggered()
{
	_toggleState = !_toggleState;

    emit stateChanged(state());
} // end onToggleBindingTriggered

void DigitalControlSlot::onMomentaryBindingIsOnChanged()
{
    _momentaryState = false;
	foreach(ControlBinding* binding, bindings())
	{
		BaseDigitalBinding* digital = (BaseDigitalBinding*) binding;
		if(!digital->toggle() && digital->isOn())
		{
			_momentaryState = true;
			break;
		} // end if
	} // end foreach

    emit stateChanged(state());
} // end onMomentaryBindingIsOnChanged

void DigitalControlSlot::onBindingAttached(ControlBinding* binding)
{
	ControlSlot::onBindingAttached(binding);
} // end onBindingAttached

void DigitalControlSlot::onBindingRemoved(ControlBinding* binding)
{
	ControlSlot::onBindingRemoved(binding);
} // end onBindingRemoved
