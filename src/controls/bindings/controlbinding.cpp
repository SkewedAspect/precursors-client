#include "controlbinding.h"

#include "controls/controlbindingmap.h"


ControlBinding::ControlBinding(ControlBindingMap* bindingMap) :
		_isActive(true),
		_map(bindingMap),
		QObject(bindingMap)
{
	connect(bindingMap, SIGNAL(isActiveChanged()), this, SIGNAL(isActiveChanged()));
} // end ControlBinding

bool ControlBinding::isActive()
{
	return _map->isActive() && _isActive;
} // end isActive

ControlBindingMap* ControlBinding::map()
{
	return _map;
} // end map
