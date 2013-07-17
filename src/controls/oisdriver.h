#ifndef OISDRIVER_H
#define OISDRIVER_H

#include "inputdriver.h"

class OISDriver : public InputDriver
{
    Q_OBJECT
public:
    explicit OISDriver(QObject *parent = 0);
};

#endif // OISDRIVER_H
