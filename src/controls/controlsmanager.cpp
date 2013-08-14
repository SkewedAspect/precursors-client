#include "controlsmanager.h"
#include "controlcontext.h"
#include "controlbindingmap.h"
#include "iinputdriver.h"

#include "controls/devices/inputdevice.h"
#include "controls/signals/inputsignal.h"
#include "controls/slots/controlslot.h"

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>
#include <QJsonDocument>
#include <QTextStream>


ControlsManager::ControlsManager(QObject* parent) :
		_logger(PLogManager::getLogger("ControlsManager")),
		_settings(PSettingsManager::instance()),
		QObject(parent)
{
} // end ControlsManager

ControlsManager& ControlsManager::instance()
{
    static ControlsManager _instance;
    return _instance;
} // end instance

const ControlsManager::ControlSlotHash ControlsManager::controlSlots() const
{
	return _controlSlots;
} // end controlSlots

const ControlsManager::InputDriverHash ControlsManager::drivers() const
{
	return _drivers;
} // end drivers

const ControlsManager::InputDeviceHash ControlsManager::devices() const
{
	return _devices;
} // end devices

const ControlsManager::ControlContextHash ControlsManager::contexts() const
{
	return _contexts;
} // end contexts

QWindow* ControlsManager::window() const
{
	return _window;
} // end window

ControlContext* ControlsManager::currentContext() const
{
	return _currentContext;
} // end setCurrentContext

void ControlsManager::setCurrentContext(ControlContext* context)
{
	if(_currentContext)
	{
		_currentContext->setIsActive(false);
	} // end if

	_currentContext = context;

	if(context)
	{
		context->setIsActive(true);
	} // end if

	_logger.info(QString("Current context changed to \"%1\"").arg(context->name()));
	emit currentContextChanged();
} // end setCurrentContext

ControlContext* ControlsManager::context(QString name)
{
	return _contexts.value(name);
} // end context

void ControlsManager::setWindow(QWindow* window)
{
	_window = window;

	foreach(IInputDriver* driver, _drivers.values())
	{
		driver->setWindow(_window);
	} // end foreach
} // end setWindow

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
	_logger.debug(QString("Device %1 attached.").arg(device->id()));

	device->loadBindings();

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
	_logger.debug(QString("Device \"%1\" (unmapped): axis \"%2\" updated: %3")
			.arg(signal->device()->id())
			.arg(signal->name())
			.arg(position)
			);
} // end onAxisSignalUpdated

void ControlsManager::onButtonSignalUpdated(bool pressed, bool repeating)
{
	InputSignal* signal = qobject_cast<InputSignal*>(sender());
	_logger.debug(QString("Device \"%1\" (unmapped): button \"%2\" updated: %3%4")
			.arg(signal->device()->id())
			.arg(signal->name())
			.arg(pressed ? "pressed" : "not pressed")
			.arg(repeating ? " (repeating)" : "")
			);
} // end onButtonSignalUpdated

bool ControlsManager::loadContextDefs(QString path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		_logger.error(QString("Couldn't open context definition file \"%1\"! (error: %2)")
				.arg(path).arg(file.error()));
		return false;
	} // end if

	// Set up a text stream
	QTextStream in(&file);

	// Read the context definitions
	QJsonParseError jsonError;
	QJsonDocument json = QJsonDocument::fromJson(in.readAll().toUtf8(), &jsonError);

	if(jsonError.error != QJsonParseError::NoError)
	{
		_logger.error(QString("Error parsing context definition file \"%1\"! (error: %2)")
				.arg(path).arg(jsonError.errorString()));
		return false;
	} // end if

	QMapIterator<QString, QVariant> contextIter(json.toVariant().toMap());
	while(contextIter.hasNext())
	{
		contextIter.next();
		ControlContext* ctx = new ControlContext(contextIter.key(), this);
		ctx->loadSlotDefinitions(contextIter.value().toMap());
		_contexts[contextIter.key()] = ctx;
		_logger.debug(QString("Loaded context \"%1\".").arg(contextIter.key()));
	} // end while

	return true;
} // end loadContextDefs
