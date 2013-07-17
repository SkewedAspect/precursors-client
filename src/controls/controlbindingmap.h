#ifndef CONTROLBINDINGMAP_H
#define CONTROLBINDINGMAP_H

#include <QObject>

#include "controlbinding.h"

class ControlBindingMap : public QObject
{
    Q_OBJECT
public:
    explicit ControlBindingMap(QObject *parent = 0);

    QString name;
    QString description;
    QList<ControlBinding*> bindings;
};

#endif // CONTROLBINDINGMAP_H
