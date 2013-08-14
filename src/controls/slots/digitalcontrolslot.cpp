#include "digitalcontrolslot.h"


DigitalControlSlot::DigitalControlSlot(QString name, ControlContext* context) :
		ControlSlot(name, context)
{
} // end DigitalControlSlot

const ControlSlot::Type DigitalControlSlot::type() const
{
	return ControlSlot::DIGITAL;
} // end type

bool DigitalControlSlot::state() const
{
	return _state;
} // end state

void DigitalControlSlot::onBindingStateChanged()
{
	//FIXME: Implement!
} // end onBindingStateChanged

void DigitalControlSlot::onBindingAttached(ControlBinding* binding)
{
	ControlSlot::onBindingAttached(binding);
} // end onBindingAttached

void DigitalControlSlot::onBindingRemoved(ControlBinding* binding)
{
	ControlSlot::onBindingRemoved(binding);
} // end onBindingRemoved
