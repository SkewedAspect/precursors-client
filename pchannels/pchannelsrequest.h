#ifndef PCHANNELSREQUEST_H
#define PCHANNELSREQUEST_H

#include <QObject>

#include "pchannels.h"

class PChannelsRequest : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QVariantMap replyMessage READ getReply)
public:
    explicit PChannelsRequest();
    explicit PChannelsRequest(PChannels* parent, QString channel, QVariant message, PChannels::ChannelMode mode);

    Q_INVOKABLE bool isOutstanding();
    Q_INVOKABLE void send(bool encrypted = true);

    // Out of a cannon!
    void fireReply();

    quint32 id;
    QString channel;
    PChannels::ChannelMode mode;
    QVariant requestMessage;
    QVariantMap replyMessage;

	QVariantMap getReply() const { return replyMessage; };

signals:
    void confirmed();
    void denied();
    void reply(bool confirmed);

private:
    PChannels* parent;
    bool _isOutstanding;
};

#endif // PCHANNELSREQUEST_H
