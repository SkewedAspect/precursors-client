#ifndef PSETTINGS_H
#define PSETTINGS_H

#include <QObject>
#include <QVariant>

#include "psettings_global.h"

class PSETTINGSSHARED_EXPORT PSettingsManager : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QVariantMap settings MEMBER _settings NOTIFY loaded)
public:
    explicit PSettingsManager(QObject *parent = 0);

    Q_INVOKABLE void reload();
    Q_INVOKABLE void save();

    Q_INVOKABLE QVariant get(QString key, QVariant defaultValue = QVariant());
    Q_INVOKABLE void set(QString key, QVariant value);
	Q_INVOKABLE void del(QString key);

private:
	QVariantMap _settings;
	QVariantMap getSettings();
	QString settingsPath;

signals:
	void loaded();
};

#endif // PSETTINGS_H

