#ifndef CONTROLCONTEXT_H
#define CONTROLCONTEXT_H

#include <QObject>

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
