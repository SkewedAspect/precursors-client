#ifndef OISDRIVER_H
#define OISDRIVER_H

#include "plogging/plogging.h"
#include "psettings/psettings.h"
#include "controls/inputdriver.h"

#include <QList>

#include <OIS.h>


class GenericDevice;
class AxisInputSignal;
class ButtonInputSignal;

class OISDriverEventHandler;

//-------------------------------------------------------------------------------------------------------------------//

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

//-------------------------------------------------------------------------------------------------------------------//

class OISDriverEventHandler : public QObject
{
	Q_OBJECT

public:
	OISDriverEventHandler(QString name);
	~OISDriverEventHandler();

	QString name;
	GenericDevice* _device;
}; // end OISDriverEventHandler

//-------------------------------------------------------------------------------------------------------------------//

class OISMouseEventHandler : public OISDriverEventHandler, public OIS::MouseListener
{
	Q_OBJECT

public:
	OISMouseEventHandler(InputDriver* driver, OIS::Mouse* oisDevice);
	~OISMouseEventHandler();

	bool mouseMoved(const OIS::MouseEvent& event);
	bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id);

protected:
	void timerEvent(QTimerEvent* event);

	OIS::Mouse* _oisDevice;

	PLogger& _logger;
}; // end OISMouseEventHandler

//-------------------------------------------------------------------------------------------------------------------//

class OISJoystickEventHandler : public OISDriverEventHandler, public OIS::JoyStickListener
{
	Q_OBJECT

public:
	OISJoystickEventHandler(InputDriver* driver, OIS::JoyStick* oisDevice);
	~OISJoystickEventHandler();

	bool buttonPressed(const OIS::JoyStickEvent& event, int buttonIndex);
	bool buttonReleased(const OIS::JoyStickEvent& event, int buttonIndex);
	bool axisMoved(const OIS::JoyStickEvent& event, int axisIndex);
	bool sliderMoved(const OIS::JoyStickEvent& event, int sliderIndex);
	bool povMoved(const OIS::JoyStickEvent& event, int povIndex);
	bool vector3Moved(const OIS::JoyStickEvent& event, int vec3Index);

protected:
	struct Hat;

	void timerEvent(QTimerEvent* event);

	OIS::JoyStick* _oisDevice;

	QList<AxisInputSignal*> _axes;
	QList<ButtonInputSignal*> _buttons;
	QList<Hat*> _hats;

	PLogger& _logger;
}; // end OISJoystickEventHandler

#endif // OISDRIVER_H
