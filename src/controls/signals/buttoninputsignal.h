#ifndef BUTTONINPUTSIGNAL_H
#define BUTTONINPUTSIGNAL_H

#include "inputsignal.h"


class ButtonInputSignal : public InputSignal
{
    Q_OBJECT

public:
    explicit ButtonInputSignal(InputDevice* device, QString name, QString description = QString());
    void emitUpdated(bool pressed, bool repeating = false);

signals:
    void updated(bool pressed, bool repeating);
};

#endif // BUTTONINPUTSIGNAL_H
