#ifndef CONTROLBINDING_H
#define CONTROLBINDING_H

#include <QObject>

#include "controlbindingmap.h"


class ControlBindingMap;


/**
 * @brief Binds an InputSignal to an ControlSlot. These objects have additional properties defining how the binding
 * should behave. (for instance, inverting the value of an axis or button, or turning a button into a toggle) These
 * properties are loaded from and saved to settings inside a ControlBindingMap.
 */
class ControlBinding : public QObject
{
    Q_OBJECT

public:
    explicit ControlBinding(QObject *parent = 0);

    bool isActive;
    ControlBindingMap* map;

signals:
    void stateChanged();
};

#endif // CONTROLBINDING_H
