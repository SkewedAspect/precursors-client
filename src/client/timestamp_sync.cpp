#include <QDateTime>

#include "timestamp_sync.h"
#include "pchannels/pchannels.h"
#include "pchannels/pchannelsrequest.h"

#define TIMEDELTA_WINDOW_SIZE 10
#define RESPONSETIME_WINDOW_SIZE 10
#define PING_FREQUENCY 5000

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

TimestampSync::TimestampSync(QObject *parent) :
    QObject(parent),
    _lastAvgResponseTime(0),
    _lastAvgTimeDelta(0),
    logger(PLogManager::getLogger("timestamp_sync"))
{
    // Start the send ping timer when we connect to the server
    connect(&PChannels::instance(), SIGNAL(connected()), this, SLOT(startPings()));

    // Stop the send ping timer when we disconnect from a server
    connect(&PChannels::instance(), SIGNAL(disconnected(QString)), this, SLOT(stopPings()));

    // Send ping requests whenever our timeer tells us to
    connect(&_sendpingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));

    // Start our ping benchmark
    _pingTime.start();
}// end TimestampSync

/**
 * @brief Get the single instance of the timestamp sync code.
 * @return Returns the static instance of the timestamp sync code.
 */
TimestampSync& TimestampSync::instance()
{
    static TimestampSync _instance;
    return _instance;
} // end instance

/**
 * @brief Gets the time delta in miliseconds between the server and client.
 * @param timestamp The unix-epoch timestamp from the server. This is assumed to be in UTC.
 * @param responseTime The time (in miliseconds) between the client sending a message, and receiving a reply.
 * (Also known as the "ping time".) This simply makes the calculation more accurate.
 * @return The time delta (in miliseconds).
 */
qint64 TimestampSync::getTimeDelta(qint64 timestamp, qint64 responseTime)
{
    if(responseTime < 0)
    {
        responseTime = calcAverageResponseTime();
    } // end if

    // This assumes `timestamp` is already in UTC.
    return (QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - (timestamp * 1000)) - (responseTime / 2);
}// end getTimeDelta

/**
 * @brief TimestampSync::getAdjustedTimeDelta
 * @param timestamp
 * @return The time delta (in miliseconds).
 */
qint64 TimestampSync::getAdjustedTimeDelta(qint64 timestamp)
{
    return this->getTimeDelta(this->getAdjustedTimestamp(timestamp));
}// end getAdjustedTimeDelta

/**
 * @brief TimestampSync::getAdjustedTimestamp
 * @param timestamp
 * @return A unix-epoch timestamp, adjusted for latency.
 */
qint64 TimestampSync::getAdjustedTimestamp(qint64 timestamp)
{
    return timestamp + calcAverageTimeDelta();
} // end getAdjustedTimestamp

/**********************************************************************************************************************/
/* C++ Only API                                                                                                       */
/**********************************************************************************************************************/

/**
 * @brief calcAverageResponseTime
 * @return
 */
qint64 TimestampSync::calcAverageResponseTime()
{
    qint64 avgRespTime = 0;

    if(_responseTimeWindow.length() >= 0)
    {
        // Sum the items in our responseTimeWindow.
        qint64 sum = 0;
        QListIterator<qint64> i(_responseTimeWindow);
        while (i.hasNext())
            sum += i.next();

        avgRespTime = sum / _responseTimeWindow.length();
    } // end if

    if(_lastAvgResponseTime != avgRespTime)
    {
        emit responseTimeChanged();
    } // end if

    return avgRespTime;
} // end calcAverageResponseTime

/**
 * @brief calcAverageTimeDelta
 * @return
 */
qint64 TimestampSync::calcAverageTimeDelta()
{
    qint64 avgTimeDelta = 0;

    if(_timeDeltaWindow.length() >= 0)
    {
        // Sum the items in our timeDeltaWindow.
        qint64 sum = 0;
        QListIterator<qint64> i(_timeDeltaWindow);
        while (i.hasNext())
            sum += i.next();

        avgTimeDelta = sum / _timeDeltaWindow.length();
    } // end if

    if(_lastAvgTimeDelta!= avgTimeDelta)
    {
        emit deltaChanged();
    } // end if

    return avgTimeDelta;
} // end calcAverageTimeDelta

/**********************************************************************************************************************/
/* Slots																											  */
/**********************************************************************************************************************/

void TimestampSync::startPings()
{
    logger.debug("Starting timestamp pings");
    _sendpingTimer.start(PING_FREQUENCY);
} // end startPing

void TimestampSync::stopPings()
{
    logger.debug("Stoping timestamp pings");
    _sendpingTimer.stop();
} // end stopPings

void TimestampSync::sendPing()
{
    QVariantMap msg;
    msg["type"] = "ping";

    // Setup a request
    PChannelsRequest* pingRequest = PChannels::instance().buildRequest("ping", msg, PChannels::CM_RELIABLE);
    connect(pingRequest, SIGNAL(reply(bool)), this, SLOT(handlePingResponse(bool)));

    _pingTime.restart();
    pingRequest->send();
} // end sendPing

void TimestampSync::handlePingResponse(bool confirmed)
{
    qint64 responseTime = _pingTime.elapsed();

    if(!confirmed)
    {
        logger.warning("Server denied ping request. This should never happen.");
    } // end if

    // Regardless, the server responded, so we can do our calculation.
    PChannelsRequest* pingReq = qobject_cast<PChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = pingReq->replyMessage;

    // Add to our response time window, but make sure we don't exceed our size.
    _responseTimeWindow.append(responseTime);
    while(_responseTimeWindow.length() > RESPONSETIME_WINDOW_SIZE)
    {
        _responseTimeWindow.removeFirst();
    } // end while

    qint64 newTimeDelta = this->getTimeDelta(replyMessage["timestamp"].toInt(), responseTime);

    // Add to our timeDelta window, but make sure we don't exceed our size.
    _timeDeltaWindow.append(newTimeDelta);
    while(_timeDeltaWindow.length() > TIMEDELTA_WINDOW_SIZE)
    {
        _timeDeltaWindow.removeFirst();
    } // end while

    //logger.debug(QString("Average Response Time: %1").arg(calcAverageResponseTime()));
    //logger.debug(QString("Average TimeDelta: %1").arg(calcAverageTimeDelta()));
} // end handlePingResponse
