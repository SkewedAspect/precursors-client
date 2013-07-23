#ifndef INPUTSIGNAL_H
#define INPUTSIGNAL_H

#include <QObject>
#include <QString>


class InputDevice;


/**
 * @brief Represents a button or axis on a given input device; tracks the current state of the button or axis, and emits
 * signals when the state changes.
 */
class InputSignal : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(InputDevice* device READ device)

public:
    explicit InputSignal(InputDevice* device, QString name, QString description = QString());

    QString name();
    QString description();
    InputDevice* device();

private:
    QString _name;
    QString _description;
    InputDevice* _device;
};

#endif // INPUTSIGNAL_H
