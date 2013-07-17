#ifndef BUTTONDIGITALBINDING_H
#define BUTTONDIGITALBINDING_H

#include "controlbinding.h"

class ButtonDigitalBinding : public ControlBinding
{
    Q_OBJECT
public:
    explicit ButtonDigitalBinding(QObject *parent = 0);

    bool toggle;
    bool invert;
    bool active;	// ReadOnly

public slots:
    void onSignalUpdated(bool pressed, bool repeating);
};

#endif // BUTTONDIGITALBINDING_H
