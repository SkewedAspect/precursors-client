#ifndef INPUTSIGNAL_H
#define INPUTSIGNAL_H

#include <QObject>
#include <QString>


class InputDevice;


/**
 * @brief Represents a button or axis on a given input device; tracks the current state of the button or axis, and emits
 * signals when the state changes.
 */
class InputSignal : public QObject
{
    Q_OBJECT

public:
	enum Type
	{
		AXIS,
		BUTTON
	}; // end Type
	Q_ENUMS(Type)

	static QString typeString(const Type);

private:
    Q_PROPERTY(Type type READ type)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(InputDevice* device READ device)

public:
    explicit InputSignal(InputDevice* device, QString name, QString description = QString());

	virtual const Type type() const = 0;
    const QString name() const;
    const QString description() const;
    InputDevice* device() const;

private:
    QString _name;
    QString _description;
    InputDevice* _device;
};

#endif // INPUTSIGNAL_H
