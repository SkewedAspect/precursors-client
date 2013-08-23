#ifndef CONTROLCONTEXT_H
#define CONTROLCONTEXT_H

#include <QHash>
#include <QObject>
#include <QString>

#include "plogging/plogging.h"


class ControlSlot;
class AnalogControlSlot;
class DigitalControlSlot;
class ControlBindingMap;


/**
 * @brief Represents a collection of ControlSlots that may be used in a given game state. For instance, there may be a
 * "flightsim" context, a "thirdperson" context, and a "UI" context.
 */
class ControlContext : public QObject
{
    Q_OBJECT

	typedef QHash<QString, ControlSlot*> ControlSlotHash;
	typedef QHash<QString, ControlBindingMap*> ControlBindingMapHash;

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
	Q_PROPERTY(ControlSlotHash controlSlots READ controlSlots)
	Q_PROPERTY(ControlBindingMapHash bindingMaps READ bindingMaps)

public:
    explicit ControlContext(QString name, QObject *parent = 0);

    const QString name() const;
    const QString description() const;
    bool isActive() const;
	const ControlSlotHash controlSlots() const;
	const ControlBindingMapHash bindingMaps() const;

	void setIsActive(bool active);

	Q_INVOKABLE AnalogControlSlot* analogSlot(QString name);
	Q_INVOKABLE DigitalControlSlot* digitalSlot(QString name);

	void loadSlotDefinitions(QVariantMap slotDefs);

signals:
    void nameChanged();
    void descriptionChanged();
    void isActiveChanged();

private:
    QString _name;
    QString _description;
    bool _isActive;

	ControlSlotHash _controlSlots;
	ControlBindingMapHash _bindingMaps;

	PLogger& _logger;
}; // end ControlContext

#endif // CONTROLCONTEXT_H
