#ifndef CONTROLBINDINGMAP_H
#define CONTROLBINDINGMAP_H

#include <QList>
#include <QObject>
#include <QString>


class ControlBinding;


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

public:
    explicit ControlBindingMap(QObject *parent = 0);

    QString name;
    QString description;
    QList<ControlBinding*> bindings;
};

#endif // CONTROLBINDINGMAP_H
