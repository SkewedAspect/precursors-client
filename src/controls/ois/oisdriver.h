#ifndef OISDRIVER_H
#define OISDRIVER_H

#include "plogging/plogging.h"
#include "psettings/psettings.h"
#include "controls/inputdriver.h"

#include <QList>

#include <OIS.h>


class GenericDevice;

class OISDriverEventHandler;


class OISDriver : public InputDriver
{
    Q_OBJECT

	Q_PLUGIN_METADATA(IID IInputDriver_IID) // FILE "oisdriver.json")
	Q_INTERFACES(IInputDriver)

public:
    explicit OISDriver();
	~OISDriver();

	virtual QString name();

    virtual void setWindow(QWindow* window);

protected:
	void teardownOIS();

private:
	OIS::InputManager* _ois;

	QHash<OIS::Object*, OISDriverEventHandler*> _devices;

	PLogger& _logger;
	PSettingsManager& _settings;
}; // end OISDriver


class OISDriverEventHandler : public QObject
{
	Q_OBJECT

public:
	OISDriverEventHandler(QString name);
	~OISDriverEventHandler();

	QString name;
	GenericDevice* device;
}; // end OISDriverEventHandler


class OISMouseEventHandler : public OISDriverEventHandler, public OIS::MouseListener
{
	Q_OBJECT

public:
	OISMouseEventHandler(InputDriver* driver, OIS::Mouse* oisDevice);
	~OISMouseEventHandler();

	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

protected:
	void timerEvent(QTimerEvent* event);

	OIS::Mouse* oisDevice;

	PLogger& _logger;
}; // end OISMouseEventHandler

class OISJoystickEventHandler : public OISDriverEventHandler, public OIS::JoyStickListener
{
	Q_OBJECT

public:
	OISJoystickEventHandler(InputDriver* driver, OIS::JoyStick* oisDevice);
	~OISJoystickEventHandler();

	bool buttonPressed(const OIS::JoyStickEvent& arg, int button);
	bool buttonReleased(const OIS::JoyStickEvent& arg, int button);
	bool axisMoved(const OIS::JoyStickEvent& arg, int axis);
	bool povMoved(const OIS::JoyStickEvent& arg, int pov);
	bool vector3Moved(const OIS::JoyStickEvent& arg, int index);

protected:
	void timerEvent(QTimerEvent* event);

	OIS::JoyStick* oisDevice;

	PLogger& _logger;
}; // end OISJoystickEventHandler

#endif // OISDRIVER_H
