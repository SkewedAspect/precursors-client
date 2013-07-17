#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <QObject>

#include "inputdriver.h"

/**
 * @brief Represents an individual input device; defines InputSignals for each axis or button on the device.
 */
class InputDevice : public QObject
{
    Q_OBJECT
public:
    explicit InputDevice(QObject *parent = 0);

    QString id;
    InputDriver* driver;
    QHash<QString, AxisInputSignal*> axisSignals;
    QHash<QString, ButtonInputSignal*> buttonSignals;
    QHash<QString, InputSignal*> inputSignals;

signals:
    void detached();
};

#endif // INPUTDEVICE_H