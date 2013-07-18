#ifndef AXISINPUTSIGNAL_H
#define AXISINPUTSIGNAL_H

#include "inputsignal.h"


class AxisInputSignal : public InputSignal
{
    Q_OBJECT

public:
    explicit AxisInputSignal(QObject *parent = 0);
};

#endif // AXISINPUTSIGNAL_H
