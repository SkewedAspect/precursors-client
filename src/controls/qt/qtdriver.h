#ifndef QTDRIVER_H
#define QTDRIVER_H

#include "controls/inputdriver.h"


class QtDriver : public InputDriver
{
    Q_OBJECT

	Q_PLUGIN_METADATA(IID IInputDriver_IID) // FILE "qtdriver.json")
	Q_INTERFACES(IInputDriver)

public:
    explicit QtDriver();

	virtual QString name();

    virtual void setWindow(QWindow* window);
};

#endif // QTDRIVER_H
