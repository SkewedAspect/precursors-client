#ifndef PCHANNELS_H
#define PCHANNELS_H

#include <QObject>
#include <QHostInfo>
#include <QHostAddress>
#include <QJsonValue>
#include <QSslSocket>
#include <QTcpSocket>
#include <QUdpSocket>

#include "pchannels_global.h"
#include "aes.h"
#include "plogging/plogging.h"

class PChannelsRequest;
class QNetString;

class PCHANNELSSHARED_EXPORT PChannels : public QObject
{
    Q_OBJECT
    Q_ENUMS(ChannelMode)
    Q_ENUMS(ErrorType)
public:
    enum ChannelMode
    {
        CM_SECURE,
        CM_RELIABLE,
        CM_UNRELIABLE
    };

    enum ErrorType
    {
        QC_GENERAL_ERROR,
        QC_CONNECTION_ERROR,
        QC_SEND_ERROR,
        QC_RECEIVE_ERROR
    };

    explicit PChannels(QObject *parent = 0);

    static PChannels& instance();

    Q_INVOKABLE void connectToServer(QString serverHostName, quint16 port, QString username, QString pwdHash);
    void disconnect(QString reason = "Quit");

    void send(QVariant envelope, ChannelMode mode, bool encrypted = true);
    void sendEvent(QString channel, QVariant message, ChannelMode mode, bool encrypted = true);
    void sendRequest(PChannelsRequest* request, bool encrypted = true);

    Q_INVOKABLE PChannelsRequest* buildRequest(QString channel, QVariant message, ChannelMode mode = CM_RELIABLE);

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
    QHash<quint32, PChannelsRequest*> requests;

    AES* cipher;

    QSslSocket* sslSocket;
    QTcpSocket* tcpSocket;
    QUdpSocket* udpSocket;

    QNetString* sslNetstring;
    QNetString* tcpNetstring;

    bool _tcpConnected;
    bool _udpConnected;

    QString sessionCookie;

    PLogger& logger;

signals:
    void error(ErrorType errorType, QString errorMessage);
    void connected();
    void disconnected(QString);
    void incomingMessage(QString channel, QVariantMap message);

private slots:
    void sslConnected();
    void tcpConnected();
    void handleLoginResponse(bool confirmed);
    void handleTCPResponse(bool confirmed);
    void handleUDPResponse(bool confirmed);

    void sslDataReady();
    void tcpDataReady();
    void udpDataReady();

    void sslError(QAbstractSocket::SocketError error);
    void tcpError(QAbstractSocket::SocketError error);
    void udpError(QAbstractSocket::SocketError error);

    void sslDisconnected();
    void tcpDisconnected();
    void udpDisconnected();

    void handleIncomingMessage(QByteArray data);
};

#endif // PCHANNELS_H
