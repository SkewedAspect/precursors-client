#ifndef CONTROLSMANAGER_H
#define CONTROLSMANAGER_H

#include <QObject>
#include <QHash>

#include "inputdriver.h"
#include "inputdevice.h"

/**
 * @brief Loads and initializes available InputDrivers and provides access to InputDevices detected by the loaded InputDrivers.
 * Also provides an API to manage ControlContexts. and loading of rom configuration; providing an API to manage them.
 */
class ControlsManager : public QObject
{
    Q_OBJECT
public:
    explicit ControlsManager(QObject *parent = 0);

    QHash<QString, InputDriver*> drivers;
    QHash<QString, InputDevice*> devices;

signals:
    void driversChanged();
    void devicesChanged();
};

#endif // CONTROLSMANAGER_H
