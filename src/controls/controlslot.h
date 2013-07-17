#ifndef CONTROLSLOT_H
#define CONTROLSLOT_H

#include <QObject>

#include "controlbinding.h"

class ControlSlot : public QObject
{
    Q_OBJECT
public:
    explicit ControlSlot(QObject *parent = 0);

    QString name;
    QString description;
    QList<ControlBinding*> bindings;
signals:
    void stateChanged();
};

#endif // CONTROLSLOT_H
