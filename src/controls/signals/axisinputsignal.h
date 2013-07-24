#ifndef AXISINPUTSIGNAL_H
#define AXISINPUTSIGNAL_H

#include "inputsignal.h"


class AxisInputSignal : public InputSignal
{
    Q_OBJECT

public:
    explicit AxisInputSignal(InputDevice* device, QString name, QString description = QString());

    void emitUpdated(float position);

signals:
    void updated(float position);
};

#endif // AXISINPUTSIGNAL_H
