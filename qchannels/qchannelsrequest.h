#ifndef QCHANNELSREQUEST_H
#define QCHANNELSREQUEST_H

#include <QObject>

#include "qchannels.h"

class QChannelsRequest : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QVariantMap replyMessage READ getReply)
public:
    explicit QChannelsRequest();
    explicit QChannelsRequest(QChannels* parent, QString channel, QVariant message, QChannels::ChannelMode mode);

    Q_INVOKABLE bool isOutstanding();
    Q_INVOKABLE void send(bool encrypted = true);

    // Out of a cannon!
    void fireReply();

    quint32 id;
    QString channel;
    QChannels::ChannelMode mode;
    QVariant requestMessage;
    QVariantMap replyMessage;

	QVariantMap getReply() const { return replyMessage; };

signals:
    void confirmed();
    void denied();
    void reply(bool confirmed);

private:
    QChannels* parent;
    bool _isOutstanding;
};

#endif // QCHANNELSREQUEST_H
