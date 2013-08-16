#ifndef DIGITALCONTROLSLOT_H
#define DIGITALCONTROLSLOT_H

#include "plogging/plogging.h"

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
	void onToggleBindingTriggered();
	void onMomentaryBindingIsOnChanged();
	virtual void onBindingAttached(ControlBinding* binding);
	virtual void onBindingRemoved(ControlBinding* binding);

signals:
    void stateChanged(bool state);

private:
    bool _toggleState;
    bool _momentaryState;

	PLogger& _logger;
}; // end DigitalControlSlot

#endif // DIGITALCONTROLSLOT_H
