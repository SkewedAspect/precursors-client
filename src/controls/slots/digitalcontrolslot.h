#ifndef DIGITALCONTROLSLOT_H
#define DIGITALCONTROLSLOT_H

#include "controlslot.h"


class DigitalControlSlot : public ControlSlot
{
    Q_OBJECT

public:
    explicit DigitalControlSlot(QObject *parent = 0);

    bool state;

signals:
    void stateChanged(bool state);
};

#endif // DIGITALCONTROLSLOT_H
