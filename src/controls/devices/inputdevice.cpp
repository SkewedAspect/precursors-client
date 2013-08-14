#include "inputdevice.h"

#include "controls/controlsmanager.h"
#include "controls/controlbindingmap.h"
#include "controls/signals/axisinputsignal.h"
#include "controls/signals/buttoninputsignal.h"

#include <QStringList>


QString InputDevice::typeString(const InputDevice::Type type)
{
	switch(type)
	{
		case MOUSE:
			return "mouse";
		case KEYBOARD:
			return "keyboard";
		case JOYSTICK:
			return "joystick";
		case OTHER:
			return "other";
		default:
			return "UNKNOWN";
	} // end switch
} // end typeString


InputDevice::InputDevice(InputDriver* driver) :
		_manager(ControlsManager::instance()),
		_settings(PSettingsManager::instance()),
		QObject((QObject*) driver)
{
} // end InputDevice

const QString InputDevice::id() const
{
	return _id;
} // end id

InputDriver* InputDevice::driver() const
{
	return _driver;
} // end driver

QVariantMap InputDevice::config() const
{
	return _settings.get("controls")
			.toMap()[typeString(type())]
			.toMap()["devices"]
			.toMap()[_id]
			.toMap();
} // end config

const InputDevice::AxisSignalHash InputDevice::axisSignals() const
{
	return _axisSignals;
} // end axisSignals

const InputDevice::ButtonSignalHash InputDevice::buttonSignals() const
{
	return _buttonSignals;
} // end buttonSignals

const InputDevice::SignalHash InputDevice::inputSignals() const
{
	SignalHash inputSignals;

	foreach(QString key, _axisSignals.keys())
	{
		inputSignals[key] = (InputSignal*) _axisSignals[key];
	} // end foreach

	foreach(QString key, _buttonSignals.keys())
	{
		inputSignals[key] = (InputSignal*) _buttonSignals[key];
	} // end foreach

	return inputSignals;
} // end inputSignals

const InputDevice::BindingMapHash InputDevice::bindingMaps() const
{
	return _bindingMaps;
} // end bindingMaps

void InputDevice::loadBindings()
{
	if(_bindingMaps.isEmpty())
	{
		// Create ControlBindingMap instances for each context.
		foreach(ControlContext* context, ControlsManager::instance().contexts())
		{
			_bindingMaps[context] = new ControlBindingMap(context, this);
		} // end foreach
	} // end if

	QVariant contextBindingMaps = config()["contexts"];
	if(!contextBindingMaps.isNull())
	{
		_logger().debug("Loading bindings...");

		QMapIterator<QString, QVariant> contextBindingMapIter(contextBindingMaps.toMap());
		while(contextBindingMapIter.hasNext())
		{
			contextBindingMapIter.next();

			ControlContext* ctx = _manager.context(contextBindingMapIter.key());

			if(ctx)
			{
				_logger().debug(QString("  Context \"%1\":").arg(contextBindingMapIter.key()));

				_bindingMaps.value(ctx)->load(contextBindingMapIter.value().toMap());
			}
			else
			{
				_logger().warn(QString("  Invalid context \"%1\" specified in configuration! Skipping.")
						.arg(contextBindingMapIter.key()));
			} // end if
		} // end while

		_logger().debug("Done.");
	}
	else
	{
		_logger().debug("No bindings defined. Available signals:");

		QStringList signalNames = axisSignals().keys();
		signalNames.sort();
		foreach(QString axisSignalName, signalNames)
		{
			_logger().debug(QString(" - axis \"%1\"").arg(axisSignalName));
		} // end foreach

		signalNames = buttonSignals().keys();
		signalNames.sort();
		foreach(QString buttonSignalName, signalNames)
		{
			_logger().debug(QString(" - button \"%1\"").arg(buttonSignalName));
		} // end foreach
	} // end if
} // end loadBindings
