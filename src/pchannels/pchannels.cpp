#include <QCoreApplication>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "preutil/preutil.h"

#include "pchannels.h"
#include "pchannelsrequest.h"
#include "qnetstring.h"

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief The default constructor.
 * @param parent The parent QObject.
 */
PChannels::PChannels(QObject *parent) :
    QObject(parent),
    logger(PLogManager::getLogger("networking"))
{
    // Setup our AES class
    this->cipher = new AES();

    // Setup SSL Netstring handler
    sslNetstring = new QNetString();
    connect(sslNetstring, SIGNAL(dataReady(QByteArray)), this, SLOT(handleIncommingMessage(QByteArray)));

    // Setup TCP Netstring handler
    tcpNetstring = new QNetString();
    connect(tcpNetstring, SIGNAL(dataReady(QByteArray)), this, SLOT(handleIncommingMessage(QByteArray)));

    // Create a new SSL socket, and conect it's signals.
    this->sslSocket = new QSslSocket(this);
    connect(this->sslSocket, SIGNAL(encrypted()), this, SLOT(sslConnected()));
    connect(this->sslSocket, SIGNAL(readyRead()), this, SLOT(sslDataReady()));
    connect(this->sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sslError(QAbstractSocket::SocketError)));
    connect(this->sslSocket, SIGNAL(disconnected()), this, SLOT(sslDisconnected()));

    // Ignore expected SSL errors
    //TODO: This should only be for debugging, not for release!
    connect(this->sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this->sslSocket, SLOT(ignoreSslErrors()));

    // Create a new TCP socket, and conect it's signals.
    this->tcpSocket = new QTcpSocket(this);
    connect(this->tcpSocket, SIGNAL(connected()), this, SLOT(tcpConnected()));
    connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpDataReady()));
    connect(this->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
    connect(this->tcpSocket, SIGNAL(disconnected()), this, SLOT(tcpDisconnected()));

    // Create a new UDP socket, and conect it's signals.
    this->udpSocket = new QUdpSocket(this);
    connect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(udpDataReady()));
    connect(this->udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(udpError(QAbstractSocket::SocketError)));
    connect(this->udpSocket, SIGNAL(disconnected()), this, SLOT(udpDisconnected()));
} // end PChannels

/**
 * @brief Get the single instance of the networking code.
 * @return Returns the static instance of the networking code.
 */
PChannels& PChannels::instance()
{
    static PChannels _instance;
    return _instance;
} // end instance

/**
 * @brief Connects to sever, on the given port.
 * @param serverHostName A string, representing the hostname of the server.
 * @param port The port to connect to the server on. (Note: this should be the ssl port.)
 */
void PChannels::connectToServer(QString serverHostName, quint16 port, QString username, QString pwdHash)
{
    // Store variable
    this->port = port;
    this->username = username;
    this->pwdHash = pwdHash;

    // Connect to the server, over ssl.
    this->sslSocket->connectToHostEncrypted(serverHostName, port);
} // end connect

/**
 * @brief Disconnects from the server.
 */
void PChannels::disconnect()
{
    // Clear our IP and port
    this->port = 0;
    this->username = "";
    this->pwdHash = "";

    // Disconnect the ssl socket if it's connected.
    if(this->sslSocket->isValid())
    {
        this->sslSocket->disconnectFromHost();
    } // end if

    // Disconnect the tcp socket if it's connected.
    if(this->tcpSocket->isValid())
    {
        this->tcpSocket->disconnectFromHost();
    } // end if

    // Disconnect the udp socket if it's connected.
    if(this->udpSocket->isValid())
    {
        this->udpSocket->disconnectFromHost();
    } // end if
} // end disconnect

/**
 * @brief PChannels::send
 * @param envelope
 * @param mode
 */
void PChannels::send(QVariant envelope, ChannelMode mode, bool encrypted)
{
    QByteArray jsonData = QJsonDocument::fromVariant(envelope).toJson(QJsonDocument::Compact);

    switch(mode)
    {
        case CM_SECURE:
        {
            QByteArray data = QNetString::encode(jsonData);
            sslSocket->write(data);
            break;
        } // end CM_SECURE

        case CM_UNRELIABLE:
        {
            QByteArray ciphertext = jsonData;
            if(encrypted)
            {
                ciphertext = cipher->encrypt(jsonData);
            } // end if

            udpSocket->writeDatagram(ciphertext, this->serverAddress, this->udpPort);
            break;
        } // end CM_UNRELIABLE

        case CM_RELIABLE:
        {
            QByteArray ciphertext = jsonData;
            if(encrypted)
            {
                ciphertext = cipher->encrypt(jsonData);
            } // end if

            QByteArray data = QNetString::encode(ciphertext);

            tcpSocket->write(data);
            break;
        } // end CM_RELIABLE

        default:
        {
            logger.critical(QString("Send with unknown mode: %1, $2, %3").arg(envelope.toString()).arg(mode).arg(encrypted));
        } // end default
    }
} // end send

/**
 * @brief PChannels::send
 * @param channel
 * @param message
 * @param mode
 */
void PChannels::sendEvent(QString channel, QVariant message, ChannelMode mode, bool encrypted)
{
    QVariantMap envelope = wrapMessage(channel, "event", message);
    send(envelope, mode, encrypted);
} // end sendEvent

/**
 * @brief PChannels::sendRequest
 * @param request
 */
void PChannels::sendRequest(PChannelsRequest* request, bool encrypted)
{
    QVariantMap envelope = wrapMessage(request->channel, "request", request->requestMessage);
    envelope["id"] = request->id;
    send(envelope, request->mode, encrypted);
} // end sendRequest

/**
 * @brief PChannels::buildRequest
 * @param channel
 * @param message
 * @param mode
 * @return
 */
PChannelsRequest* PChannels::buildRequest(QString channel, QVariant message, ChannelMode mode)
{
    // Build a new PChannelsRequest, get a new id for it and add it to our list of outstanding requests.
    PChannelsRequest* request = new PChannelsRequest(this, channel, message, mode);
    request->id = getNextID();
    this->requests.insert(request->id, request);

    return request;
} // end buildRequest

/**********************************************************************************************************************/
/* Private API                                                                                                        */
/**********************************************************************************************************************/

// Get the next available request id.
quint32 PChannels::getNextID()
{
    // In the really obscure case wherewe roll over our 32bit id, we just keep incremeting till we find an id not
    // currently in use. If we have 4,294,967,295 outstanding requests, we've got problem.
    do
    {
        this->idCounter++;
    }
    while(requests.contains(this->idCounter));

    return this->idCounter++;
} // end getNextID()

// Handle an incoming reply.
void PChannels::handleReply(QVariantMap envelope)
{
    PChannelsRequest* request = requests.take(envelope["id"].toUInt());
    request->replyMessage = envelope["contents"].toMap();
    request->fireReply();

    // Delete thi object after the handlers are done with it.
    request->deleteLater();
} // end handleReply

// Handle an incomming event.
void PChannels::handleEvent(QVariantMap envelope)
{
	emit incommingMessage(envelope["channel"].toString(), envelope["contents"].toMap());
} // end handleEvent

// Connect the TCP and UDP transports to the server.
void PChannels::connectTransports()
{
    tcpSocket->connectToHost(this->serverAddress, this->tcpPort);
    udpSocket->bind(QHostAddress::Any);

    // Send the UDP login message
    QVariantMap msg;
    msg["type"] = "connect";
    msg["cookie"] = this->sessionCookie;

    // Setup a request
    PChannelsRequest* udpRequest = buildRequest("control", msg, CM_UNRELIABLE);
    connect(udpRequest, SIGNAL(reply(bool)), this, SLOT(handleUDPResponse(bool)));

    // Send the udp request
    udpRequest->send();
} // end connectTransports

// Wraps the mesage in an envelope for sending
QVariantMap PChannels::wrapMessage(QString channel, QString type, QVariant message)
{
    QVariantMap envelope;
    envelope["type"] = type;
    envelope["channel"] = channel;
    envelope["contents"] = message;

    return envelope;
} // end wrapMessage

/**********************************************************************************************************************/
/* Private Slots                                                                                                      */
/**********************************************************************************************************************/

void PChannels::sslConnected()
{
    // Store the IP address of the server.
    this->serverAddress = this->sslSocket->peerAddress();

    QList<QVariant> version = PreUtil::instance().getClientVersionAsList();

    // Build the login message
    QVariantMap msg;
    msg["type"] = "login";
    msg["user"] = this->username;
    msg["password"] = this->pwdHash;

    msg["version"] = version;
    msg["clientName"] = QCoreApplication::instance()->applicationName();

    // Send our AES key/iv
    msg["vector"] = cipher->iv.toBase64();
    msg["key"] = cipher->key.toBase64();

    // Setup a request
    PChannelsRequest* loginRequest = buildRequest("control", msg, CM_SECURE);
    connect(loginRequest, SIGNAL(reply(bool)), this, SLOT(handleLoginResponse(bool)));

    // Send the login request
    loginRequest->send();
} // end sslConnected

void PChannels::tcpConnected()
{
    // Send the TCP login message
    QVariantMap msg;
    msg["type"] = "connect";
    msg["cookie"] = this->sessionCookie;

    // Setup a request
    PChannelsRequest* tcpRequest = buildRequest("control", msg, CM_RELIABLE);
    connect(tcpRequest, SIGNAL(reply(bool)), this, SLOT(handleTCPResponse(bool)));

    // Send the tcp request
    tcpRequest->send(false);
} // end tcpConnected

void PChannels::handleLoginResponse(bool confirmed)
{
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    PChannelsRequest* loginReq = qobject_cast<PChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = loginReq->replyMessage;

    if(!confirmed)
    {
        logger.notice(QString("Server denied login: \"%1\".").arg(replyMessage["reason"].toString()));
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
        this->tcpPort = replyMessage["tcpPort"].toUInt();
        this->udpPort = replyMessage["udpPort"].toUInt();

        this->sessionCookie = replyMessage["cookie"].toString();

        // Connect our TCP and UDP sockets.
        connectTransports();
    } // end if
} // end handleLoginResponse

void PChannels::handleTCPResponse(bool confirmed)
{
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    PChannelsRequest* tcpReq = qobject_cast<PChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = tcpReq->replyMessage;

    if(!confirmed)
    {
        logger.error(QString("Server denied tcp connection: \"%1\".").arg(replyMessage["reason"].toString()));
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
        this->_tcpConnected = true;
        if(this->_udpConnected)
        {
            emit connected();
        } // end if
    } // end if
} // end handleTCPResponse

void PChannels::handleUDPResponse(bool confirmed)
{
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    PChannelsRequest* udpReq = qobject_cast<PChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = udpReq->replyMessage;

    if(!confirmed)
    {
        logger.error(QString("Server denied udp connection: \"%1\".").arg(replyMessage["reason"].toString()));
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
		logger.debug("UDP RESPONSE CONFIRMED!");
        this->_udpConnected = true;
        if(this->_tcpConnected)
        {
            emit connected();
        } // end if
    } // end if
} // end handleUDPResponse

void PChannels::handleIncommingMessage(QByteArray data)
{
    QByteArray plainText = data;

    if(qobject_cast<QNetString*>(QObject::sender()) == this->tcpNetstring)
    {
        plainText = cipher->decrypt(data);
    } // end if

    QVariantMap envelope = QJsonDocument::fromJson(plainText).toVariant().toMap();

    if(envelope["type"] == "response")
    {
        handleReply(envelope);
        return;
    } // end if

    if(envelope["type"] == "event")
    {
        handleEvent(envelope);
        return;
    } // end if

    logger.warning(QString("Received incoming message with unknown type: \"%1\".").arg(envelope["type"].toString()));
} // end handleIncommingMessage

void PChannels::sslDataReady()
{
    sslNetstring->addData(sslSocket->readAll());
} // end sslDataReady

void PChannels::tcpDataReady()
{
    tcpNetstring->addData(tcpSocket->readAll());
} // end tcpDataReady

void PChannels::udpDataReady()
{
    while (udpSocket->hasPendingDatagrams())
    {
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            // Decrypt AES
            QByteArray plainText = cipher->decrypt(datagram);

			QVariantMap envelope = QJsonDocument::fromJson(plainText).toVariant().toMap();

            // Now, parse as JSON, and emit.
            emit incommingMessage(envelope["channel"].toString(), envelope["contents"].toMap());
    } // end while
} // end udpDataReady


void PChannels::sslError(QAbstractSocket::SocketError error)
{
    logger.error(QString("SSL Error: \"%1\".").arg(sslSocket->errorString()));
} // end sslDebug

void PChannels::tcpError(QAbstractSocket::SocketError error)
{
    logger.error(QString("TCP Error: \"%1\".").arg(tcpSocket->errorString()));
} // end sslDebug
void PChannels::udpError(QAbstractSocket::SocketError error)
{
    logger.error(QString("UDP Error: \"%1\".").arg(udpSocket->errorString()));
} // end sslError

void PChannels::sslDisconnected()
{
    disconnect();
} // end sslDisconnected

void PChannels::tcpDisconnected()
{
    disconnect();
} // end sslDisconnected

void PChannels::udpDisconnected()
{
    disconnect();
} // end sslDisconnected

/**********************************************************************************************************************/
