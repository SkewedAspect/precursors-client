#ifndef INPUTSIGNAL_H
#define INPUTSIGNAL_H

#include <QObject>

#include "inputdevice.h"

/**
 * @brief Represents a button or axis on a given input device; tracks the current state of the button or axis, and emits
 * signals when the state changes.
 */
class InputSignal : public QObject
{
    Q_OBJECT
public:
    explicit InputSignal(QObject *parent = 0);

    QString name;
    QString description;
    InputDevice* device;
signals:

public slots:

};

#endif // INPUTSIGNAL_H
