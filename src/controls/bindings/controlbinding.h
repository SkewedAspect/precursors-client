#ifndef CONTROLBINDING_H
#define CONTROLBINDING_H

#include <QObject>


class ControlBindingMap;


/**
 * @brief Binds an InputSignal to an ControlSlot. These objects have additional properties defining how the binding
 * should behave. (for instance, inverting the value of an axis or button, or turning a button into a toggle) These
 * properties are loaded from and saved to settings inside a ControlBindingMap.
 */
class ControlBinding : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

public:
	typedef QList<ControlBinding*> List;

    explicit ControlBinding(ControlBindingMap* bindingMap);

    bool isActive();
    ControlBindingMap* map();

	virtual bool configure(QVariantMap bindingDef) = 0;

signals:
	void isActiveChanged();

private:
    bool _isActive;
    ControlBindingMap* _map;
}; // end ControlBinding

#endif // CONTROLBINDING_H
