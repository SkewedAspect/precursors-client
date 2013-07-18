#ifndef QTDRIVER_H
#define QTDRIVER_H

#include "inputdriver.h"


class QtDriver : public InputDriver
{
    Q_OBJECT

public:
    explicit QtDriver(QObject *parent = 0);
};

#endif // QTDRIVER_H
