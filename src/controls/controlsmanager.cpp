#include "controlsmanager.h"


ControlsManager::ControlsManager(QObject *parent) :
    QObject(parent)
{
}

void ControlsManager::onDeviceAttached(InputDevice* device)
{
    emit devicesChanged();
} // end onDeviceAttached

void ControlsManager::onDeviceDetached(InputDevice* device)
{
    emit devicesChanged();
} // end onDeviceDetached
