#ifndef DIGITALCONTROLSLOT_H
#define DIGITALCONTROLSLOT_H

#include "controlslot.h"


class DigitalControlSlot : public ControlSlot
{
    Q_OBJECT

    Q_PROPERTY(bool state READ state NOTIFY stateChanged)

public:
    explicit DigitalControlSlot(QString name, ControlContext* context);

	virtual const ControlSlot::Type type() const;

    bool state() const;

public slots:
	virtual void onBindingStateChanged();
	virtual void onBindingAttached(ControlBinding* binding);
	virtual void onBindingRemoved(ControlBinding* binding);

signals:
    void stateChanged(bool state);

private:
    bool _toggleState;
    bool _state;
}; // end DigitalControlSlot

#endif // DIGITALCONTROLSLOT_H
