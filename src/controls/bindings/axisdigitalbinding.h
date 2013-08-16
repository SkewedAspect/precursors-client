#ifndef AXISDIGITALBINDING_H
#define AXISDIGITALBINDING_H

#include "basedigitalbinding.h"


/**
 * @brief A binding between an axis signal and a digital input slot.
 *
 * This binding type takes 4 options:
 *
 * - `threshold`: The value over which this binding is considered "active". (or inactive, if `invert` is true)
 * - `overlap`: The difference between the triggering threshold and the un-triggering threshold. (to overcome jitter)
 * - `toggle`: If true, invert the slot's value when we transition from a "low" value to a "high" value.
 * - `invert`: If true, trigger the binding on a "low" value (or "high" to "low" transition) instead of a "high" value
 *     (or "low" to "high" transition).
 */
class AxisDigitalBinding : public BaseDigitalBinding
{
    Q_OBJECT

    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(float overlap READ overlap WRITE setOverlap NOTIFY overlapChanged)
	Q_PROPERTY(float lowerThreshold READ lowerThreshold NOTIFY triggerRangeChanged)
	Q_PROPERTY(float upperThreshold READ upperThreshold NOTIFY triggerRangeChanged)

public:
    explicit AxisDigitalBinding(ControlBindingMap* bindingMap);

    float threshold();
    float overlap();
	float lowerThreshold();
	float upperThreshold();

    void setThreshold(float threshold);
    void setOverlap(float overlap);

	virtual bool configure(QVariantMap bindingDef);

signals:
	void thresholdChanged();
	void overlapChanged();
	void triggerRangeChanged();

public slots:
    void onSignalUpdated(float position);

private:
	void updateTriggerRange();

    float _threshold;
    float _overlap;
	float _lowerThreshold;
	float _upperThreshold;
}; // end AxisDigitalBinding

#endif // AXISDIGITALBINDING_H
