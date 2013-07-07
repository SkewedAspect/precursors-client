#ifndef PREUTIL_H
#define PREUTIL_H

#include <QObject>
#include <QVariant>

#include "plogging/plogging.h"
#include "preutil_global.h"

class PREUTILSHARED_EXPORT PreUtil : public QObject
{
    Q_OBJECT
public:
    explicit PreUtil();

	static PreUtil& instance();

	Q_INVOKABLE QString getClientVersion();
	Q_INVOKABLE QList<QVariant> getClientVersionAsList();

private:
	PLogger& logger;
};

#endif // PREUTIL_H


