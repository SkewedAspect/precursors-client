#ifndef BUTTONINPUTSIGNAL_H
#define BUTTONINPUTSIGNAL_H

#include "inputsignal.h"

class ButtonInputSignal : public InputSignal
{
    Q_OBJECT
public:
    explicit ButtonInputSignal(QObject *parent = 0);
};

#endif // BUTTONINPUTSIGNAL_H
