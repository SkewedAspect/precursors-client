#ifndef AXISDIGITALBINDING_H
#define AXISDIGITALBINDING_H

#include "controlbinding.h"


class AxisDigitalBinding : public ControlBinding
{
    Q_OBJECT

public:
    explicit AxisDigitalBinding(QObject *parent = 0);

    float threshold;
    float overlap;
    bool invert;
    bool state;	// Read only

public slots:
    void onSignalUpdated(float position);
};

#endif // AXISDIGITALBINDING_H
