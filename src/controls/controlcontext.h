#ifndef CONTROLCONTEXT_H
#define CONTROLCONTEXT_H

#include <QObject>

/**
 * @brief Represents a collection of ControlSlots that may be used in a given game state. For instance, there may be a
 * "flightsim" context, a "thirdperson" context, and a "UI" context.
 */
class ControlContext : public QObject
{
    Q_OBJECT
public:
    explicit ControlContext(QObject *parent = 0);

    QString name;
    QString description;
    bool isActive;

signals:
    void activated();
    void deactivated();
};

#endif // CONTROLCONTEXT_H
