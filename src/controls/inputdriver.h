#ifndef INPUTDRIVER_H
#define INPUTDRIVER_H

#include <QObject>

class InputDriver : public QObject
{
    Q_OBJECT
public:
    explicit InputDriver(QObject *parent = 0);

    QString name;

signals:
    void deviceAttached(InputDevice*);
    void deviceDetached(InputDevice*);
};

#endif // INPUTDRIVER_H
