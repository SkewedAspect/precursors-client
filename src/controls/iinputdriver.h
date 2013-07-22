#ifndef IINPUTDRIVER_H
#define IINPUTDRIVER_H

#include <QString>


class QWindow;

class ControlsManager;
class InputDevice;


class IInputDriver
{
public:
    virtual ~IInputDriver() {}

	virtual QString name() = 0;
    virtual ControlsManager* manager() = 0;
    virtual QWindow* window() = 0;

    virtual bool activate(ControlsManager* manager) = 0;
    virtual bool deactivate() = 0;

    virtual void setWindow(QWindow* window) = 0;
};

#define IInputDriver_IID "com.skewedaspect.rfi.Precursors.IInputDriver"

Q_DECLARE_INTERFACE(IInputDriver, IInputDriver_IID)

#endif // IINPUTDRIVER_H
