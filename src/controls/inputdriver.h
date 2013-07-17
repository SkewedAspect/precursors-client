#ifndef INPUTDRIVER_H
#define INPUTDRIVER_H

#include <QObject>

/**
 * @brief Provides access to InputDevice instances for a given type of device attached to the system, and emits signals
 * for newly-connected devices when possible. Used internally by ControlsManager to determine the list of available devices.
 */
class InputDriver : public QObject
{
    Q_OBJECT
public:
    explicit InputDriver(QObject *parent = 0);

    QString name;

signals:
    void deviceAttached(InputDevice*);
    void deviceDetached(InputDevice*);
};

#endif // INPUTDRIVER_H
