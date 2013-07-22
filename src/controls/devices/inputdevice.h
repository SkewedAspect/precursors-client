#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <QHash>
#include <QObject>
#include <QString>


class AxisInputSignal;
class ButtonInputSignal;
class InputDriver;
class InputSignal;


/**
 * @brief Represents an individual input device; defines InputSignals for each axis or button on the device.
 */
class InputDevice : public QObject
{
    Q_OBJECT

	typedef QHash<QString, AxisInputSignal*> AxisSignalHash;
	typedef QHash<QString, ButtonInputSignal*> ButtonSignalHash;
	typedef QHash<QString, InputSignal*> SignalHash;

	Q_PROPERTY(QString id READ id)
    Q_PROPERTY(InputDriver* driver READ driver)
    Q_PROPERTY(AxisSignalHash axisSignals READ axisSignals)
    Q_PROPERTY(ButtonSignalHash buttonSignals READ buttonSignals)
    Q_PROPERTY(SignalHash inputSignals READ inputSignals)

public:
    explicit InputDevice(QObject *parent = 0);

    QString id();
    InputDriver* driver();
    AxisSignalHash axisSignals();
    ButtonSignalHash buttonSignals();
    SignalHash inputSignals();

signals:
    void detached();

private:
    QString _id;
    InputDriver* _driver;
    AxisSignalHash _axisSignals;
    ButtonSignalHash _buttonSignals;
    SignalHash _inputSignals;
};

#endif // INPUTDEVICE_H
