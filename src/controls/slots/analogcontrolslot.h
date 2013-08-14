#ifndef ANALOGCONTROLSLOT_H
#define ANALOGCONTROLSLOT_H

#include "controlslot.h"


class AnalogControlSlot : public ControlSlot
{
    Q_OBJECT

    Q_PROPERTY(float instantaneousValue READ instantaneousValue)
    Q_PROPERTY(float accumulatedValue READ accumulatedValue)
    Q_PROPERTY(float value READ value NOTIFY valueChanged)

public:
	explicit AnalogControlSlot(QString name, ControlContext* context);

	virtual const ControlSlot::Type type() const;

    float instantaneousValue() const;
    float accumulatedValue() const;
    float value() const;

public slots:
	virtual void onBindingStateChanged();
	virtual void onBindingAttached(ControlBinding* binding);
	virtual void onBindingRemoved(ControlBinding* binding);

signals:
    void valueChanged(float value);

private:
    float _instantaneousValue;
    float _accumulatedValue;
    float _value;
}; // end AnalogControlSlot

#endif // ANALOGCONTROLSLOT_H
