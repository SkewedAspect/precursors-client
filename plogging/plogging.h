#ifndef PLOGGING_H
#define PLOGGING_H

#include <QObject>
#include <QVariant>

#include "plogging_global.h"

class PLOGGINGSHARED_EXPORT PLogger : public QObject
{
    Q_OBJECT
public:
	QString name;

    explicit PLogger(QString name = "app");

    Q_INVOKABLE void debug(QString message);
	Q_INVOKABLE void debug(QString message, QVariantList args);

    Q_INVOKABLE void info(QString message);
    Q_INVOKABLE void info(QString message, QVariantList args);

    Q_INVOKABLE void notice(QString message);
    Q_INVOKABLE void notice(QString message, QVariantList args);

    Q_INVOKABLE void notify(QString message);
    Q_INVOKABLE void notify(QString message, QVariantList args);

    Q_INVOKABLE void warning(QString message);
    Q_INVOKABLE void warning(QString message, QVariantList args);

    Q_INVOKABLE void warn(QString message);
    Q_INVOKABLE void warn(QString message, QVariantList args);

    Q_INVOKABLE void error(QString message);
    Q_INVOKABLE void error(QString message, QVariantList args);

    Q_INVOKABLE void critical(QString message);
    Q_INVOKABLE void critical(QString message, QVariantList args);

    Q_INVOKABLE void fatal(QString message);
    Q_INVOKABLE void fatal(QString message, QVariantList args);

private:
	QString buildString(QString level, QString message);
	void log(QString level, QString message);
};

class PLOGGINGSHARED_EXPORT PLogManager : public QObject
{
    Q_OBJECT
public:
    explicit PLogManager(QObject* parent = 0);

	static PLogManager& instance();

	Q_INVOKABLE PLogger* logger(QString name = "app");
	static PLogger& getLogger(QString name = "app");

private:
	QMap<QString, PLogger*> _loggers;
};

#endif // PLOGGING_H

