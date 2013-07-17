#ifndef TIMESTAMP_SYNC_H
#define TIMESTAMP_SYNC_H

#include <QObject>
#include <QQueue>
#include <QTimer>

#include "plogging/plogging.h"

class TimestampSync : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 avgResponseTime READ calcAverageResponseTime NOTIFY responseTimeChanged)
    Q_PROPERTY(qint64 avgTimeDelta READ calcAverageTimeDelta NOTIFY deltaChanged)

public:
    explicit TimestampSync(QObject *parent = 0);

    static TimestampSync& instance();

    Q_INVOKABLE qint64 getTimeDelta(qint64 timestamp, qint64 responseTime = -1);
    Q_INVOKABLE qint64 getAdjustedTimeDelta(qint64 timestamp);
    Q_INVOKABLE qint64 getAdjustedTimestamp(qint64 timestamp);

    qint64 calcAverageResponseTime();
    qint64 calcAverageTimeDelta();

signals:
    void deltaChanged();
    void responseTimeChanged();

public slots:
    void startPings();
    void stopPings();

    void sendPing();
    void handlePingResponse(bool confirmed);

private:
    PLogger& logger;

    qint64 _lastAvgResponseTime;
    qint64 _lastAvgTimeDelta;

    QTime _pingTime;
    QTimer _sendpingTimer;
    QQueue<qint64> _timeDeltaWindow;
    QQueue<qint64> _responseTimeWindow;
};

#endif // TIMESTAMP_SYNC_H
