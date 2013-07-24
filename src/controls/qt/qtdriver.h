#ifndef QTDRIVER_H
#define QTDRIVER_H

#include <QSize>
#include <QPoint>

#include "plogging/plogging.h"
#include "psettings/psettings.h"
#include "controls/inputdriver.h"


class QMouseEvent;
class QScreen;

class GenericDevice;
class AxisInputSignal;
class ButtonInputSignal;


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
	QSize _screenSize;

	GenericDevice* _mouseDevice;
	QHash<QString, AxisInputSignal*> _mouseAxes;
	QHash<Qt::MouseButton, ButtonInputSignal*> _mouseButtons;

	PLogger& _logger;
	PSettingsManager& _settings;
};

#endif // QTDRIVER_H
