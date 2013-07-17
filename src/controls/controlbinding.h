#ifndef CONTROLBINDING_H
#define CONTROLBINDING_H

#include <QObject>

#include "controlbindingmap.h"

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
