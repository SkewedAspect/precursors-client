#ifndef AXISANALOGBINDING_H
#define AXISANALOGBINDING_H

#include "plogging/plogging.h"

#include "controlbinding.h"


/**
 * @brief A binding between an axis signal and an analog input slot.
 */
class AxisAnalogBinding : public ControlBinding
{
    Q_OBJECT

    Q_PROPERTY(float value READ value NOTIFY setTo)

    Q_PROPERTY(float sensitivity MEMBER _sensitivity NOTIFY sensitivityChanged)
    Q_PROPERTY(float offset MEMBER _offset NOTIFY offsetChanged)
    Q_PROPERTY(float deadZone MEMBER _deadZone NOTIFY deadZoneChanged)
	Q_PROPERTY(bool delta READ isDelta WRITE setDelta NOTIFY deltaChanged)

public:
    explicit AxisAnalogBinding(ControlBindingMap* bindingMap);

	float value();
	bool isDelta();

	void setDelta(bool isDelta);

	virtual bool configure(QVariantMap bindingDef);

signals:
    void setTo(float value);
	void changeRateSet();

    void sensitivityChanged();
    void offsetChanged();
    void deadZoneChanged();
    void deltaChanged();

public slots:
    void onSignalUpdated(float position);

private:
    float _value;

    float _sensitivity;
    float _offset;
    float _deadZone;
	bool _isDelta;

	PLogger& _logger;
}; // end AxisAnalogBinding

#endif // AXISANALOGBINDING_H
