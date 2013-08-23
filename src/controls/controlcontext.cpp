#include "controlcontext.h"
#include "slots/analogcontrolslot.h"
#include "slots/digitalcontrolslot.h"

#include <QVariantMap>


ControlContext::ControlContext(QString name, QObject *parent) :
		_logger(PLogManager::getLogger(QString("ControlContext[%1]").arg(name))),
		_name(name),
		_description(""),
		_isActive(false),
		QObject(parent)
{
} // end ControlContext

const QString ControlContext::name() const
{
	return _name;
} // end name

const QString ControlContext::description() const
{
	return _description;
} // end description

bool ControlContext::isActive() const
{
	return _isActive;
} // end isActive

const ControlContext::ControlSlotHash ControlContext::controlSlots() const
{
	return _controlSlots;
} // end controlSlots

const ControlContext::ControlBindingMapHash ControlContext::bindingMaps() const
{
	return _bindingMaps;
} // end bindingMaps

void ControlContext::setIsActive(bool active)
{
	_isActive = active;
	emit isActiveChanged();
} // end setIsActive

AnalogControlSlot* ControlContext::analogSlot(QString name)
{
	AnalogControlSlot* slot = qobject_cast<AnalogControlSlot*>(_controlSlots.value(name));

	if(!slot)
	{
		_logger.error(QString("Invalid analog slot \"%1\" requested!").arg(name));
	} // end if

	return slot;
} // end analogSlot

DigitalControlSlot* ControlContext::digitalSlot(QString name)
{
	DigitalControlSlot* slot = qobject_cast<DigitalControlSlot*>(_controlSlots.value(name));

	if(!slot)
	{
		_logger.error(QString("Invalid digital slot \"%1\" requested!").arg(name));
	} // end if

	return slot;
} // end digitalSlot

void ControlContext::loadSlotDefinitions(QVariantMap slotDefs)
{
	QMapIterator<QString, QVariant> slotsIter(slotDefs);
	while(slotsIter.hasNext())
	{
		slotsIter.next();
		QString slotType = slotsIter.value().toString();
		if(slotType == "analog")
		{
			_controlSlots[slotsIter.key()] = new AnalogControlSlot(slotsIter.key(), this);
		}
		else if(slotType == "digital")
		{
			_controlSlots[slotsIter.key()] = new DigitalControlSlot(slotsIter.key(), this);
		}
		else
		{
			_logger.error(QString("Unrecognized slot type \"%1\"!").arg(slotType));
		} // end if
	} // end while
} // end if
