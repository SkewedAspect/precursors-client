#ifndef CONTROLSMANAGER_H
#define CONTROLSMANAGER_H

#include <QObject>
#include <QHash>

#include "inputdriver.h"
#include "inputdevice.h"

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
