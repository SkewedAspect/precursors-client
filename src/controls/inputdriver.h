#ifndef INPUTDRIVER_H
#define INPUTDRIVER_H

#include <QObject>

#include "plogging/plogging.h"
#include "iinputdriver.h"


/**
 * @brief Provides access to InputDevice instances for a given type of device attached to the system, and emits signals
 * for newly-connected devices when possible. Used internally by ControlsManager to determine the list of available devices.
 */
class InputDriver : public QObject, public IInputDriver
{
    Q_OBJECT

	Q_INTERFACES(IInputDriver)

    Q_PROPERTY(QString name READ name);
    Q_PROPERTY(QWindow* window READ window WRITE setWindow NOTIFY windowChanged)

public:
    explicit InputDriver();

	virtual QString name() = 0;
    virtual ControlsManager* manager();
    virtual QWindow* window();
	virtual QObject* qObject();

    virtual void setWindow(QWindow* window) = 0;

    virtual bool activate(ControlsManager* manager);
    virtual bool deactivate();

signals:
    void deviceAttached(InputDevice*);
    void deviceDetached(InputDevice*);
	void windowChanged();

protected:
	ControlsManager* _manager;
	QWindow* _window;

	PLogger& _logger;
}; // end InputDriver

#endif // INPUTDRIVER_H
