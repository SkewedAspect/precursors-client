#ifndef BUTTONDIGITALBINDING_H
#define BUTTONDIGITALBINDING_H

#include "controlbinding.h"


/**
 * @brief A binding between a button signal and a digital input slot.
 */
class ButtonDigitalBinding : public ControlBinding
{
    Q_OBJECT

    Q_PROPERTY(bool isOn READ isOn NOTIFY isOnChanged)

    Q_PROPERTY(bool invert MEMBER _invert NOTIFY invertChanged)
    Q_PROPERTY(bool toggle MEMBER _toggle NOTIFY toggleChanged)

public:
    explicit ButtonDigitalBinding(QObject *parent = 0);

    bool isOn();

signals:
    void isOnChanged();
    void invertChanged();
    void toggleChanged();

public slots:
    void onSignalUpdated(bool pressed, bool repeating);

private:
    bool _isOn;

    bool _invert;
    bool _toggle;

    bool _lastPressedState;
}; // end ButtonDigitalBinding

#endif // BUTTONDIGITALBINDING_H
