#ifndef OISDRIVER_H
#define OISDRIVER_H

#include "plogging/plogging.h"
#include "psettings/psettings.h"
#include "controls/inputdriver.h"


namespace OIS
{
	class InputManager;
}

class OISDriver : public InputDriver
{
    Q_OBJECT

public:
    explicit OISDriver(ControlsManager* manager, QWindow* window);
	~OISDriver();

	virtual QString name();

private:
	OIS::InputManager* ois;

	PLogger& _logger;
	PSettingsManager& _settings;
}; // end OISDriver

#endif // OISDRIVER_H
