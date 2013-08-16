#ifndef BASEDIGITALBINDING_H
#define BASEDIGITALBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between a button signal and a digital input slot.
 */
class BaseDigitalBinding : public ControlBinding
{
    Q_OBJECT

    Q_PROPERTY(bool isOn READ isOn NOTIFY isOnChanged)

    Q_PROPERTY(bool invert READ invert WRITE setInvert NOTIFY invertChanged)
    Q_PROPERTY(bool toggle READ toggle WRITE setToggle NOTIFY toggleChanged)

public:
    explicit BaseDigitalBinding(ControlBindingMap* bindingMap);

    bool isOn();

    bool invert();
    bool toggle();

    void setInvert(bool invert);
    void setToggle(bool toggle);

	virtual bool configure(QVariantMap bindingDef);

signals:
    void stateChanged();

	/**
	 * If this is a momentary (non-toggle) binding, this is connected to the control slot's momentaryUpdated() slot.
	 */
    void isOnChanged();

	/**
	 * If this is a toggle binding, this is connected to the control slot's toggle() slot.
	 */
    void triggered();

    void invertChanged();
    void toggleChanged();

protected:
	bool inputState();
	void setInputState(bool state);

private:
    bool _isOn;

    bool _invert;
    bool _toggle;

	bool _lastInputState;
}; // end BaseDigitalBinding

#endif // BASEDIGITALBINDING_H
