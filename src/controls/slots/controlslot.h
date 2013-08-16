#ifndef CONTROLSLOT_H
#define CONTROLSLOT_H

#include <QList>
#include <QObject>
#include <QString>

#include "controls/bindings/controlbinding.h"


class ControlContext;


/**
 * @brief Represents an action that may be taken in response to input (DigitalControlSlot), or a quantity that can be
 * varied according to input. (AnalogControlSlot) Client code should instantiate these in order to handle input.
 */
class ControlSlot : public QObject
{
	Q_OBJECT

public:
	enum Type
	{
		ANALOG,
		DIGITAL
	}; // end Type
	Q_ENUMS(Type)

	static QString typeString(const Type);

private:
    Q_PROPERTY(Type type READ type)
	Q_PROPERTY(QString name READ name)
	Q_PROPERTY(ControlContext* context READ context)
	Q_PROPERTY(QString description READ description)
	Q_PROPERTY(ControlBinding::List bindings READ bindings NOTIFY bindingsChanged)

public:
	explicit ControlSlot(QString name, ControlContext* context);

	virtual const Type type() const = 0;
	const QString name() const;
	ControlContext* context() const;
	const QString description() const;
	const ControlBinding::List bindings() const;

public slots:
	virtual void onBindingAttached(ControlBinding* binding);
	virtual void onBindingRemoved(ControlBinding* binding);

signals:
	void bindingsChanged();

private:
	QString _name;
	ControlContext* _context;
	QString _description;
	ControlBinding::List _bindings;
}; // end ControlSlot

#endif // CONTROLSLOT_H
