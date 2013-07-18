#ifndef QTDRIVER_H
#define QTDRIVER_H

#include "controls/inputdriver.h"


class QtDriver : public InputDriver
{
    Q_OBJECT

public:
    explicit QtDriver(ControlsManager* manager, QWindow* window);

	virtual QString name();
};

#endif // QTDRIVER_H
