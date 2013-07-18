#ifndef BUTTONDIGITALBINDING_H
#define BUTTONDIGITALBINDING_H

#include "controlbinding.h"


class ButtonDigitalBinding : public ControlBinding
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool invert MEMBER _invert NOTIFY invertChanged)
    Q_PROPERTY(bool toggle MEMBER _toggle NOTIFY toggleChanged)

public:
    explicit ButtonDigitalBinding(QObject *parent = 0);

    bool state();

signals:
    void invertChanged();
    void toggleChanged();

public slots:
    void onSignalUpdated(bool pressed, bool repeating);

private:
    bool _invert;
    bool _toggle;
    bool _state;
    bool _lastPressedState;
};

#endif // BUTTONDIGITALBINDING_H
