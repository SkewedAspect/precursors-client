#ifndef AXISINPUTSIGNAL_H
#define AXISINPUTSIGNAL_H

#include <QVariantMap>

#include "psettings/psettings.h"

#include "inputsignal.h"


class AxisInputSignal : public InputSignal
{
    Q_OBJECT

public:
    explicit AxisInputSignal(InputDevice* device, QString name, QString description = QString());

	virtual const InputSignal::Type type() const;
    float centerValue() const;
    float deadZone() const;
    float maxValue() const;
    float minValue() const;
	QVariantMap calibration() const;

    void setCenterValue(float value);
    void setDeadZone(float value);
    void setMaxValue(float value);
    void setMinValue(float value);

    void emitUpdated(float position);

signals:
    void updated(float position);

private:
    float _centerValue;
    float _deadZone;
    float _maxValue;
    float _minValue;

	PSettingsManager& _settings;
};

#endif // AXISINPUTSIGNAL_H
