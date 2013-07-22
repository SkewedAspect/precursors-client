#ifndef OISDRIVER_H
#define OISDRIVER_H

#include <QList>

#include "plogging/plogging.h"
#include "psettings/psettings.h"
#include "controls/inputdriver.h"


namespace OIS
{
	class InputManager;
	class Object;
}

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

	QList<OIS::Object*> devices;

	PLogger& _logger;
	PSettingsManager& _settings;
}; // end OISDriver

#endif // OISDRIVER_H
