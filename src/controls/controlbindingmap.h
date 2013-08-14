#ifndef CONTROLBINDINGMAP_H
#define CONTROLBINDINGMAP_H

#include <QList>
#include <QObject>
#include <QString>

#include "plogging/plogging.h"

#include "controls/bindings/controlbinding.h"


class ControlContext;
class InputDevice;
class ControlBinding;
class InputSignal;
class ControlSlot;


/**
 * @brief Binding maps allow different sets of bindings to be stored and chosen via settings. For instance, a keyboard
 * may have separate ControlBindingMaps for QWERTY and Dvorak layouts, while a gamepad may have a "Normal" binding map,
 * and a "Southpaw" binding map.
 *
 * A ControlBindingMap contains a set of ControlBindings for each available ControlContext. Each ControlBindingMap is
 * specific to a given type of InputDevice, and may be copied to a new one and edited as desired, or modified in-place.
 * The user may only choose one ControlBindingMap at a time for a given InputDevice.
 */
class ControlBindingMap : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
	Q_PROPERTY(ControlContext* context READ context)
	Q_PROPERTY(InputDevice* device READ device)
    Q_PROPERTY(ControlBinding::List bindings READ bindings NOTIFY bindingsChanged)

public:
    explicit ControlBindingMap(ControlContext* context, InputDevice* device);

    bool isActive() const;
	ControlContext* context() const;
	InputDevice* device() const;
    const ControlBinding::List bindings() const;

	Q_INVOKABLE void load(QVariantMap bindings);

signals:
	void isActiveChanged();
	void bindingsChanged();

private:
	ControlBinding* createBinding(InputSignal* inputSignal, ControlSlot* controlSlot);

	ControlContext* _context;
	InputDevice* _device;
    ControlBinding::List _bindings;

	PLogger& _logger;
}; // end ControlBindingMap

#endif // CONTROLBINDINGMAP_H
