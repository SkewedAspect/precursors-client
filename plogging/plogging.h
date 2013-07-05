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
	QString buildLogString(QString level, QString message);
	const char * buildLogMessage(QString message);
};

#endif // PLOGGING_H

