#ifndef QCHANNELS_H
#define QCHANNELS_H

#include <QObject>
#include <QHostInfo>
#include <QHostAddress>
#include <QJsonValue>
#include <QSslSocket>
#include <QTcpSocket>
#include <QUdpSocket>

#include <QtCrypto>

#include "qchannels_global.h"

class QChannelsRequest;
class QNetString;

class QCHANNELSSHARED_EXPORT QChannels : public QObject
{
    Q_OBJECT
public:
    enum ChannelMode
    {
        CM_SECURE,
        CM_RELIABLE,
        CM_UNRELABLE
    };

    enum ErrorType
    {
        QC_GENERAL_ERROR,
        QC_CONNECTION_ERROR,
        QC_SEND_ERROR,
        QC_RECEIVE_ERROR
    };

    explicit QChannels(QObject *parent = 0);

    void connectToServer(QString serverHostName, quint16 port, QString username, QString pwdHash);
    void disconnect();

    void send(QVariant envelope, ChannelMode mode);
    void sendEvent(QString channel, QVariant message, ChannelMode mode);
    void sendRequest(QChannelsRequest* request);

    QChannelsRequest* buildRequest(QString channel, QVariant message, ChannelMode mode);

    QHostAddress serverAddress;
    quint16 port;

    quint16 tcpPort;
    quint16 udpPort;

    QString username;
    QString pwdHash;

private:
    quint32 getNextID();
    void handleReply(QVariantMap envelope);
    void handleEvent(QVariantMap envelope);
    void connectTransports();
    QVariantMap wrapMessage(QString channel, QString type, QVariant message);

    quint32 idCounter;
    QHash<quint32, QChannelsRequest*> requests;

	QCA::SymmetricKey key;
	QCA::InitializationVector iv;
	QCA::Cipher aes128Enc;
	QCA::Cipher aes128Dec;

    QSslSocket* sslSocket;
    QTcpSocket* tcpSocket;
    QUdpSocket* udpSocket;

    QNetString* sslNetstring;
    QNetString* tcpNetstring;

    bool _tcpConnected;
    bool _udpConnected;

    QString sessionCookie;

signals:
    void error(ErrorType errorType, QString errorMessage);
    void connected();
    void disconnected(QString);
    void incommingMessage(QVariantMap message);

private slots:
    void sslConnected();
    void tcpConnected();
    void handleLoginResponse(bool confirmed);
    void handleTCPResponse(bool confirmed);
    void handleUDPResponse(bool confirmed);

    void sslDataReady();
    void tcpDataReady();
    void udpDataReady();

    void handleIncommingMessage(QByteArray data);
};

#endif // QCHANNELS_H
