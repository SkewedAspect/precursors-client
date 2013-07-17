#ifndef DIGITALCONTROLSLOT_H
#define DIGITALCONTROLSLOT_H

#include "controlslot.h"

class DigitalControlSlot : public ControlSlot
{
    Q_OBJECT
public:
    explicit DigitalControlSlot(QObject *parent = 0);

    //XXX: Frankly, I question the name 'active'. Try: 'pressed', 'triggered', or 'activated'. Calling it "active" sounds
    // more like you're saying, "This slot has the potential to be used.", not "This slot is currently in a state of activation."
    bool active;

signals:
    void activeChanged(bool pressed);
};

#endif // DIGITALCONTROLSLOT_H
