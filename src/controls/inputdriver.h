#ifndef INPUTDRIVER_H
#define INPUTDRIVER_H

#include <QObject>
#include <QString>


class QWindow;

class ControlsManager;
class InputDevice;


/**
 * @brief Provides access to InputDevice instances for a given type of device attached to the system, and emits signals
 * for newly-connected devices when possible. Used internally by ControlsManager to determine the list of available devices.
 */
class InputDriver : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name);

public:
    explicit InputDriver(ControlsManager* manager, QWindow* window);

	virtual QString name() = 0;

signals:
    void deviceAttached(InputDevice*);
    void deviceDetached(InputDevice*);
}; // end InputDriver

#endif // INPUTDRIVER_H
