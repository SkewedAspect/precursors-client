#ifndef QCHANNELSREQUEST_H
#define QCHANNELSREQUEST_H

#include <QObject>

#include "qchannels.h"

class QChannelsRequest : public QObject
{
    Q_OBJECT
public:
    explicit QChannelsRequest(QChannels* parent, QString channel, QVariant message, QChannels::ChannelMode mode);

    bool isOutstanding();
    void send();

    quint32 id;
    QString channel;
    QChannels::ChannelMode mode;
    QVariant requestMessage;
    QVariant replyMessage;

signals:
    void confirmed();
    void denied();
    void reply(bool confirmed);

private:
    QChannels* parent;
    bool _isOutstanding;
};

#endif // QCHANNELSREQUEST_H