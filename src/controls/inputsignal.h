#ifndef INPUTSIGNAL_H
#define INPUTSIGNAL_H

#include <QObject>

#include "inputdevice.h"

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
