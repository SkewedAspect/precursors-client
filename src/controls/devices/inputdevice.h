#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QVariantMap>

#include "psettings/psettings.h"
#include "controls/controlsmanager.h"


class AxisInputSignal;
class ButtonInputSignal;
class InputDriver;
class InputSignal;
class ControlContext;
class ControlBindingMap;


/**
 * @brief Represents an individual input device; defines InputSignals for each axis or button on the device.
 */
class InputDevice : public QObject
{
    Q_OBJECT

public:
	enum Type
	{
		KEYBOARD,
		MOUSE,
		JOYSTICK,
		OTHER
	}; // end Type
	Q_ENUMS(Type)

	typedef QHash<QString, AxisInputSignal*> AxisSignalHash;
	typedef QHash<QString, ButtonInputSignal*> ButtonSignalHash;
	typedef QHash<QString, InputSignal*> SignalHash;
	typedef QHash<ControlContext*, ControlBindingMap*> BindingMapHash;

	static QString typeString(const Type);

private:
    Q_PROPERTY(Type type READ type)
	Q_PROPERTY(QString id READ id)
    Q_PROPERTY(InputDriver* driver READ driver)
    Q_PROPERTY(AxisSignalHash axisSignals READ axisSignals)
    Q_PROPERTY(ButtonSignalHash buttonSignals READ buttonSignals)
    Q_PROPERTY(SignalHash inputSignals READ inputSignals)
	Q_PROPERTY(BindingMapHash bindingMaps READ bindingMaps)

public:
    explicit InputDevice(InputDriver* driver);

	virtual const Type type() const = 0;
    virtual const QString id() const;
    virtual InputDriver* driver() const;
	QVariantMap config() const;
    const AxisSignalHash axisSignals() const;
    const ButtonSignalHash buttonSignals() const;
    const SignalHash inputSignals() const;
	const BindingMapHash bindingMaps() const;

	void loadBindings();

signals:
    void detached();
	void currentMapChanged(QString mapName);

protected:
	virtual PLogger& _logger() const = 0;

    QString _id;
    InputDriver* _driver;
    AxisSignalHash _axisSignals;
    ButtonSignalHash _buttonSignals;
	BindingMapHash _bindingMaps;

	ControlsManager& _manager;
	PSettingsManager& _settings;
}; // end InputDevice

#endif // INPUTDEVICE_H
