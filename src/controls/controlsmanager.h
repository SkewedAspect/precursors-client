#ifndef CONTROLSMANAGER_H
#define CONTROLSMANAGER_H

#include <QHash>
#include <QObject>
#include <QString>

#include "plogging/plogging.h"
#include "psettings/psettings.h"


class QWindow;

class IInputDriver;
class InputDevice;
class ControlContext;
class ControlSlot;


/**
 * @brief Loads and initializes available InputDrivers and provides access to InputDevices detected by the loaded InputDrivers.
 * Also provides an API to manage ControlContexts. and loading of rom configuration; providing an API to manage them.
 */
class ControlsManager : public QObject
{
    Q_OBJECT

public:
	typedef QHash<QString, ControlSlot*> ControlSlotHash;
	typedef QHash<QString, IInputDriver*> InputDriverHash;
	typedef QMultiHash<QString, InputDevice*> InputDeviceHash;
	typedef QHash<QString, ControlContext*> ControlContextHash;

private:
	Q_PROPERTY(ControlSlotHash controlSlots READ controlSlots)
    Q_PROPERTY(InputDriverHash drivers READ drivers NOTIFY driversChanged)
    Q_PROPERTY(InputDeviceHash devices READ devices NOTIFY devicesChanged)
    Q_PROPERTY(ControlContextHash contexts READ contexts NOTIFY contextsChanged)
    Q_PROPERTY(QWindow* window READ window WRITE setWindow NOTIFY windowChanged)
	Q_PROPERTY(ControlContext* currentContext READ currentContext WRITE setCurrentContext NOTIFY currentContextChanged)

public:
    explicit ControlsManager(QObject* parent = 0);

    static ControlsManager& instance();

	const ControlSlotHash controlSlots() const;
	const InputDriverHash drivers() const;
    const InputDeviceHash devices() const;
    const ControlContextHash contexts() const;
    QWindow* window() const;
	ControlContext* currentContext() const;

	Q_INVOKABLE ControlContext* context(QString name);

	bool loadContextDefs(QString path);

    void setWindow(QWindow* window);
	void setCurrentContext(ControlContext* context);
	Q_INVOKABLE void setCurrentContext(QString contextName);

	bool loadInputDriver(QString driverFileName);
	QStringList findInputDrivers();

signals:
    void driversChanged();
    void devicesChanged();
	void contextsChanged();
	void windowChanged();
	void currentContextChanged();

public slots:
    void onDeviceAttached(InputDevice* device);
    void onDeviceDetached(InputDevice* device);

	// Debugging
	void onAxisSignalUpdated(float position);
	void onButtonSignalUpdated(bool pressed, bool repeating);

private:
	ControlSlotHash _controlSlots;
    InputDriverHash _drivers;
    InputDeviceHash _devices;
	ControlContextHash _contexts;
	QWindow* _window;
	ControlContext* _currentContext;

	PLogger& _logger;
	PSettingsManager& _settings;
}; // end ControlsManager

#endif // CONTROLSMANAGER_H
