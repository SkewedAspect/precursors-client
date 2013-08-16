#include "controlbindingmap.h"
#include "controlcontext.h"

#include "controls/devices/inputdevice.h"
#include "controls/signals/inputsignal.h"
#include "controls/slots/controlslot.h"

#include "controls/bindings/axisanalogbinding.h"
#include "controls/bindings/axisdigitalbinding.h"
#include "controls/bindings/buttonanalogbinding.h"
#include "controls/bindings/buttondigitalbinding.h"


ControlBindingMap::ControlBindingMap(ControlContext* context, InputDevice* device) :
		_context(context),
		_device(device),
		_logger(PLogManager::getLogger(QString("ControlBindingMap[\"%1\" on \"%2\"]").arg(context->name()).arg(device->id()))),
		QObject((QObject*) device)
{
	connect(context, SIGNAL(isActiveChanged()), this, SIGNAL(isActiveChanged()));
} // end ControlBindingMap

bool ControlBindingMap::isActive() const
{
	return _context->isActive();
} // end isActive

ControlContext* ControlBindingMap::context() const
{
	return _context;
} // end context

InputDevice* ControlBindingMap::device() const
{
	return _device;
} // end device

const ControlBinding::List ControlBindingMap::bindings() const
{
	return _bindings;
} // end bindings

void ControlBindingMap::load(QVariantMap bindings)
{
	QMapIterator<QString, QVariant> bindingIter(bindings);
	while(bindingIter.hasNext())
	{
		bindingIter.next();
		InputSignal* inputSignal = _device->inputSignals().value(bindingIter.key(), NULL);

		if(!inputSignal)
		{
			_logger.error(QString("Unrecognized input signal \"%1\"!").arg(bindingIter.key()));
			continue;
		} // end if

		QVariantMap bindingDef = bindingIter.value().toMap();
		ControlSlot* controlSlot = _context->controlSlots().value(bindingDef["slot"].toString());

		if(!controlSlot)
		{
			_logger.error(QString("Unrecognized control slot \"%1\"!").arg(bindingDef["slot"].toString()));
			continue;
		} // end if

		_logger.debug(QString("   - %1 signal \"%2\" => %3 slot \"%4\"")
				.arg(InputSignal::typeString(inputSignal->type()))
				.arg(inputSignal->name())
				.arg(ControlSlot::typeString(controlSlot->type()))
				.arg(controlSlot->name())
				);

		ControlBinding* binding = createBinding(inputSignal, controlSlot);
		if(binding)
		{
			binding->configure(bindingDef);
		} // end if
	} // end while
} // end load

ControlBinding* ControlBindingMap::createBinding(InputSignal* inputSignal, ControlSlot* controlSlot)
{
	ControlBinding* binding = NULL;
	switch(inputSignal->type())
	{
		case InputSignal::AXIS:
			switch(controlSlot->type())
			{
				case ControlSlot::ANALOG:
					binding = new AxisAnalogBinding(this);
					connect(binding, SIGNAL(setTo(float)), controlSlot, SLOT(onBindingSetTo(float)));
					break;
				case ControlSlot::DIGITAL:
					binding = new AxisDigitalBinding(this);
					connect(binding, SIGNAL(stateChanged()), controlSlot, SLOT(onBindingStateChanged()));
					break;
				default:
					_logger.error(QString("Unrecognized ControlSlot type: %1").arg(controlSlot->type()));
					return NULL;
			} // end switch

			connect(inputSignal, SIGNAL(updated(float)), binding, SLOT(onSignalUpdated(float)));
			break;

		case InputSignal::BUTTON:
			switch(controlSlot->type())
			{
				case ControlSlot::ANALOG:
					binding = new ButtonAnalogBinding(this);
					connect(binding, SIGNAL(momentaryStateSet()), controlSlot, SLOT(onBindingMomentaryStateSet()));
					connect(binding, SIGNAL(changeRateSet()), controlSlot, SLOT(onBindingChangeRateSet()));
					connect(binding, SIGNAL(setTo(float)), controlSlot, SLOT(onBindingSetTo(float)));
					break;
				case ControlSlot::DIGITAL:
					binding = new ButtonDigitalBinding(this);
					connect(binding, SIGNAL(isOnChanged()), controlSlot, SLOT(onMomentaryBindingIsOnChanged()));
					connect(binding, SIGNAL(triggered()), controlSlot, SLOT(onToggleBindingTriggered()));
					break;
				default:
					_logger.error(QString("Unrecognized ControlSlot type: %1").arg(controlSlot->type()));
					return NULL;
			} // end switch

			connect(inputSignal, SIGNAL(updated(bool, bool)), binding, SLOT(onSignalUpdated(bool, bool)));
			break;

		default:
			_logger.error(QString("Unrecognized InputSignal type: %1").arg(inputSignal->type()));
			return NULL;
	} // end switch

	controlSlot->onBindingAttached(binding);

	return binding;
} // end createBinding
