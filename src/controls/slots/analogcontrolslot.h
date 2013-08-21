#ifndef ANALOGCONTROLSLOT_H
#define ANALOGCONTROLSLOT_H

#include <QTime>

#include "plogging/plogging.h"

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

	/// For momentary bindings, where the states of the bindings directly determine the value.
    float instantaneousValue() const;

	/// For most types of bindings, which modify the current "accumulated" value of the slot.
    float accumulatedValue() const;

	/// The actual calculated value.
    float value() const;

public slots:
	void onBindingMomentaryStateSet();
	void onBindingChangeRateSet();
	void onBindingSetTo(float val);
	virtual void onBindingAttached(ControlBinding* binding);
	virtual void onBindingRemoved(ControlBinding* binding);

signals:
    void valueChanged(float value);

protected:
	virtual void timerEvent(QTimerEvent* event);

private:
	void updateValue();

    float _instantaneousValue;
    float _accumulatedValue;
    float _accumulatedRateOfChange;
    float _value;

	bool _timerRunning;
	int _timerID;
	QTime _timeSinceLastROC;

	PLogger& _logger;
}; // end AnalogControlSlot

#endif // ANALOGCONTROLSLOT_H
