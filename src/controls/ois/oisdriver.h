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


class OISDriverEventHandler : public QObject, public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
	Q_OBJECT

public:
	OISDriverEventHandler(GenericDevice* device, OIS::Object* oisDevice);
	~OISDriverEventHandler();

	bool keyPressed(const OIS::KeyEvent& arg);
	bool keyReleased(const OIS::KeyEvent& arg);
	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool buttonPressed(const OIS::JoyStickEvent& arg, int button);
	bool buttonReleased(const OIS::JoyStickEvent& arg, int button);
	bool axisMoved(const OIS::JoyStickEvent& arg, int axis);
	bool povMoved(const OIS::JoyStickEvent& arg, int pov);
	bool vector3Moved(const OIS::JoyStickEvent& arg, int index);

protected:
	void timerEvent(QTimerEvent* event);

	GenericDevice* device;
	OIS::Object* oisDevice;

	PLogger& _logger;
}; // end OISDriverEventHandler

#endif // OISDRIVER_H
