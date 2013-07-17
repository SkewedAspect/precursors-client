#ifndef CONTROLSLOT_H
#define CONTROLSLOT_H

#include <QList>
#include <QObject>
#include <QString>


class ControlBinding;

/**
 * @brief Represents an action that may be taken in response to input (DigitalControlSlot), or a quantity that can be
 * varied according to input. (AnalogControlSlot) Client code should instantiate these in order to handle input.
 */
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
