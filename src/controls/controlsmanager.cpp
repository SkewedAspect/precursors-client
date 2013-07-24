#include "controlsmanager.h"
#include "iinputdriver.h"
#include "controls/signals/axisinputsignal.h"
#include "controls/signals/buttoninputsignal.h"

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>


ControlsManager::ControlsManager(QObject* parent) :
		_logger(PLogManager::getLogger("ControlsManager")),
		QObject(parent)
{
} // end ControlsManager

void ControlsManager::setWindow(QWindow* window)
{
	_window = window;

	foreach(IInputDriver* driver, _drivers.values())
	{
		driver->setWindow(_window);
	} // end foreach
} // end setWindow

QWindow* ControlsManager::window()
{
	return _window;
} // end window

bool ControlsManager::loadInputDriver(QString driverFileName)
{
	if(_drivers.contains(driverFileName))
	{
		return true;
	} // end if

	QPluginLoader pluginLoader(driverFileName);

	QObject* plugin = pluginLoader.instance();
	if(plugin)
	{
		IInputDriver* inputDriver = qobject_cast<IInputDriver*>(plugin);
		if(inputDriver)
		{
			connect(inputDriver->qObject(), SIGNAL(deviceAttached(InputDevice*)),
					this, SLOT(onDeviceAttached(InputDevice*)));
			connect(inputDriver->qObject(), SIGNAL(deviceDetached(InputDevice*)),
					this, SLOT(onDeviceDetached(InputDevice*)));

			_drivers[driverFileName] = inputDriver;

			if(_window)
			{
				inputDriver->setWindow(_window);
			} // end if

			return true;
		} // end if
	} // end if

	_logger.error(QString("Couldn't load input driver plugin \"%1\"!").arg(driverFileName));
	return false;
} // end loadInputDriver

QStringList ControlsManager::findInputDrivers()
{
	QStringList inputDriverFilenames;

	foreach(const QString& path, QCoreApplication::instance()->libraryPaths())
	{
		foreach(const QString& fileName, QDir(path).entryList(QDir::Files | QDir::Readable))
		{
			if(QLibrary::isLibrary(fileName))
			{
				//FIXME: We need to check and see if this particular library implements InputDriver! ("Use
				// QPluginLoader in the application to load the plugins."; "Use qobject_cast() to test whether a plugin
				// implements a given interface.")
				inputDriverFilenames.append(fileName);
			} // end if
		} // end foreach
	} // end foreach

	return inputDriverFilenames;
} // end findInputDrivers

void ControlsManager::onDeviceAttached(InputDevice* device)
{
	_logger.debug(QString("Device %1 attached; connecting signals...").arg(device->id()));

	foreach(AxisInputSignal* axisSignal, device->axisSignals().values())
	{
		_logger.debug(QString(" - axis signal %2").arg(axisSignal->name()));
		connect(axisSignal, SIGNAL(updated(float)), this, SLOT(onAxisSignalUpdated(float)));
	} // end foreach

	foreach(ButtonInputSignal* buttonSignal, device->buttonSignals().values())
	{
		_logger.debug(QString(" - button signal %2").arg(buttonSignal->name()));
		connect(buttonSignal, SIGNAL(updated(bool, bool)), this, SLOT(onButtonSignalUpdated(bool, bool)));
	} // end foreach

	_devices.insert(device->id(), device);
    emit devicesChanged();
} // end onDeviceAttached

void ControlsManager::onDeviceDetached(InputDevice* device)
{
	disconnect(device);
	_devices.remove(device->id(), device);
    emit devicesChanged();
} // end onDeviceDetached

void ControlsManager::onAxisSignalUpdated(float position)
{
	InputSignal* signal = qobject_cast<InputSignal*>(sender());
	_logger.debug(QString("Device \"%1\": axis \"%2\" updated: %3")
			.arg(signal->device()->id())
			.arg(signal->name())
			.arg(position)
			);
} // end onAxisSignalUpdated

void ControlsManager::onButtonSignalUpdated(bool pressed, bool repeating)
{
	InputSignal* signal = qobject_cast<InputSignal*>(sender());
	_logger.debug(QString("Device \"%1\": button \"%2\" updated: %3%4")
			.arg(signal->device()->id())
			.arg(signal->name())
			.arg(pressed ? "pressed" : "not pressed").arg(repeating ? " (repeating)" : "")
			);
} // end onButtonSignalUpdated
