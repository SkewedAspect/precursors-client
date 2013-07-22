#include "controlsmanager.h"
#include "iinputdriver.h"

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
		driver->setWindow(window);
	} // end foreach
} // end setWindow

QWindow* ControlsManager::window()
{
	return _window;
} // end window

bool ControlsManager::loadInputDriver(QString driverFileName)
{
	QPluginLoader pluginLoader(driverFileName);

	QObject* plugin = pluginLoader.instance();
	if(plugin)
	{
		IInputDriver* inputDriver = qobject_cast<IInputDriver*>(plugin);
		if(inputDriver)
		{
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
	_devices.insert(device->id(), device);
    emit devicesChanged();
} // end onDeviceAttached

void ControlsManager::onDeviceDetached(InputDevice* device)
{
	_devices.remove(device->id(), device);
    emit devicesChanged();
} // end onDeviceDetached
