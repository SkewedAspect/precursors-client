#ifndef CONTROLSMANAGER_H
#define CONTROLSMANAGER_H

#include <QHash>
#include <QObject>
#include <QString>

#include "plogging/plogging.h"
#include "devices/inputdevice.h"


class QWindow;

class IInputDriver;
class InputDevice;


/**
 * @brief Loads and initializes available InputDrivers and provides access to InputDevices detected by the loaded InputDrivers.
 * Also provides an API to manage ControlContexts. and loading of rom configuration; providing an API to manage them.
 */
class ControlsManager : public QObject
{
    Q_OBJECT

	typedef QHash<QString, IInputDriver*> InputDriverHash;
	typedef QMultiHash<QString, InputDevice*> InputDeviceHash;

    Q_PROPERTY(InputDriverHash drivers MEMBER _drivers NOTIFY driversChanged)
    Q_PROPERTY(InputDeviceHash devices MEMBER _devices NOTIFY devicesChanged)
    Q_PROPERTY(QWindow* window READ window WRITE setWindow NOTIFY windowChanged)

public:
    explicit ControlsManager(QObject* parent = 0);

    QWindow* window();

    void setWindow(QWindow* window);

	bool loadInputDriver(QString driverFileName);
	QStringList findInputDrivers();

signals:
    void driversChanged();
    void devicesChanged();
	void windowChanged();

public slots:
    void onDeviceAttached(InputDevice* device);
    void onDeviceDetached(InputDevice* device);

private:
    InputDriverHash _drivers;
    InputDeviceHash _devices;
	QWindow* _window;

	PLogger& _logger;
}; // end ControlsManager

#endif // CONTROLSMANAGER_H
