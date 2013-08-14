#include "controlslot.h"
#include "../controlcontext.h"


QString ControlSlot::typeString(ControlSlot::Type type)
{
	switch(type)
	{
		case ANALOG:
			return "analog";
		case DIGITAL:
			return "digital";
		default:
			return "UNKNOWN";
	} // end switch
} // end typeString


ControlSlot::ControlSlot(QString name, ControlContext* context) :
		_name(name),
		_context(context),
		QObject(context)
{
} // end ControlSlot

const QString ControlSlot::name() const
{
	return _name;
} // end name

ControlContext* ControlSlot::context() const
{
	return _context;
} // end context

const QString ControlSlot::description() const
{
	return _description;
} // end description

const ControlBinding::List ControlSlot::bindings() const
{
	return _bindings;
} // end bindings

void ControlSlot::onBindingAttached(ControlBinding* binding)
{
	_bindings.append(binding);
	emit bindingsChanged();
} // end onBindingAttached

void ControlSlot::onBindingRemoved(ControlBinding* binding)
{
	_bindings.removeAll(binding);
	emit bindingsChanged();
} // end onBindingRemoved
