#ifndef QTDRIVER_H
#define QTDRIVER_H

#include <QSize>
#include <QPoint>

#include "plogging/plogging.h"
#include "psettings/psettings.h"
#include "controls/inputdriver.h"


class QMouseEvent;
class QScreen;


class QtDriver : public InputDriver
{
    Q_OBJECT

	Q_PLUGIN_METADATA(IID IInputDriver_IID) // FILE "qtdriver.json")
	Q_INTERFACES(IInputDriver)

public:
    explicit QtDriver();

	virtual QString name();

    virtual void setWindow(QWindow* window);

public slots:
	void onScreenChanged(QScreen* screen);

	void onMouseMoved(QMouseEvent* event, QPoint screenDelta);
	void onMousePressed(QMouseEvent* event);
	void onMouseReleased(QMouseEvent* event);

protected:
	QSize screenSize;

	PLogger& _logger;
	PSettingsManager& _settings;
};

#endif // QTDRIVER_H
