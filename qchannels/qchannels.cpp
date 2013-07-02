#include <QJsonDocument>

#include "qchannels.h"
#include "qchannelsrequest.h"
#include "qnetstring.h"

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

/**
 * @brief The default constructor.
 * @param parent The parent QObject.
 */
QChannels::QChannels(QObject *parent) :
	QObject(parent)
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
} // end QChannels

/**
 * @brief Connects to sever, on the given port.
 * @param serverHostName A string, representing the hostname of the server.
 * @param port The port to connect to the server on. (Note: this should be the ssl port.)
 */
void QChannels::connectToServer(QString serverHostName, quint16 port, QString username, QString pwdHash)
{
	qDebug("connectToServer...");

    // Store variable
    this->port = port;
    this->username = username;
    this->pwdHash = pwdHash;

    // Connect to the server, over ssl.
    this->sslSocket->connectToHostEncrypted(serverHostName, port);

	qDebug("connectToServer finished");
} // end connect

/**
 * @brief Disconnects from the server.
 */
void QChannels::disconnect()
{
	qDebug("Disconnect called!");

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
 * @brief QChannels::send
 * @param envelope
 * @param mode
 */
void QChannels::send(QVariant envelope, ChannelMode mode, bool encrypted)
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

			qDebug() << "msg: " << ciphertext << "server: " << serverAddress << "port: " << udpPort;

            udpSocket->writeDatagram(ciphertext, this->serverAddress, this->udpPort);
            break;
        } // end CM_UNRELIABLE

        case CM_RELIABLE:
        default:
        {
			QByteArray ciphertext = jsonData;
			if(encrypted)
			{
				ciphertext = cipher->encrypt(jsonData);
			} // end if

			QByteArray data = QNetString::encode(ciphertext);

			qDebug() << "netstring:" << QString(data);

            tcpSocket->write(data);
            break;
        } // end CM_RELIABLE
    }
} // end send

/**
 * @brief QChannels::send
 * @param channel
 * @param message
 * @param mode
 */
void QChannels::sendEvent(QString channel, QVariant message, ChannelMode mode, bool encrypted)
{
    QVariantMap envelope = wrapMessage(channel, "event", message);
    send(envelope, mode, encrypted);
} // end sendEvent

/**
 * @brief QChannels::sendRequest
 * @param request
 */
void QChannels::sendRequest(QChannelsRequest* request, bool encrypted)
{
    QVariantMap envelope = wrapMessage(request->channel, "request", request->requestMessage);

	qDebug() << "Sending request:" << envelope;

	envelope["id"] = request->id;
    send(envelope, request->mode, encrypted);
} // end sendRequest

/**
 * @brief QChannels::buildRequest
 * @param channel
 * @param message
 * @param mode
 * @return
 */
QChannelsRequest* QChannels::buildRequest(QString channel, QVariant message, ChannelMode mode)
{
    // Build a new QChannelsRequest, get a new id for it and add it to our list of outstanding requests.
    QChannelsRequest* request = new QChannelsRequest(this, channel, message, mode);
    request->id = getNextID();
    this->requests.insert(request->id, request);

    return request;
} // end buildRequest

/**********************************************************************************************************************/
/* Private API                                                                                                        */
/**********************************************************************************************************************/

// Get the next available request id.
quint32 QChannels::getNextID()
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
void QChannels::handleReply(QVariantMap envelope)
{
    QChannelsRequest* request = requests.take(envelope["id"].toUInt());
    request->replyMessage = envelope["contents"].toMap();
    request->fireReply();

    // Delete thi object after the handlers are done with it.
    request->deleteLater();
} // end handleReply

// Handle an incomming event.
void QChannels::handleEvent(QVariantMap envelope)
{
    emit incommingMessage(envelope["content"].toMap());
} // end handleEvent

// Connect the TCP and UDP transports to the server.
void QChannels::connectTransports()
{
	qDebug("connectTransports...");
    tcpSocket->connectToHost(this->serverAddress, this->tcpPort);
    udpSocket->bind(this->serverAddress, this->udpPort);

    // Send the UDP login message
    QVariantMap msg;
    msg["type"] = "connect";
    msg["cookie"] = this->sessionCookie;

    // Setup a request
    QChannelsRequest* udpRequest = buildRequest("control", msg, CM_UNRELIABLE);
    connect(udpRequest, SIGNAL(reply(bool)), this, SLOT(handleUDPResponse(bool)));

    // Send the udp request
    udpRequest->send(false);
	qDebug("connectTransports done");
} // end connectTransports

// Wraps the mesage in an envelope for sending
QVariantMap QChannels::wrapMessage(QString channel, QString type, QVariant message)
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

void QChannels::sslConnected()
{
	qDebug("sslConnected...");
	//
    // Store the IP address of the server.
    this->serverAddress = this->sslSocket->peerAddress();

    // Buid our version list
    QList<QVariant> version;
    version.append(0);						// Major
    version.append(5);						// Minor
    version.append(0);						// Micro
    version.append("Client PreAlpha 1");	// Release name

    // Build the login message
    QVariantMap msg;
    msg["type"] = "login";
    msg["user"] = this->username;
    msg["password"] = this->pwdHash;

    //TODO: We should get this information somehow. Perhaps from a header?
    msg["version"] = version;
    msg["clientName"] = "Official Precursors Client";

    // Send our AES key/iv
    msg["vector"] = cipher->iv.toBase64();
    msg["key"] = cipher->key.toBase64();

    // Setup a request
    QChannelsRequest* loginRequest = buildRequest("control", msg, CM_SECURE);
    connect(loginRequest, SIGNAL(reply(bool)), this, SLOT(handleLoginResponse(bool)));

    // Send the login request
    loginRequest->send();

	qDebug("sslConnected finished.");
} // end sslConnected

void QChannels::tcpConnected()
{
	qDebug("tcpConnected...");
    // Send the TCP login message
    QVariantMap msg;
    msg["type"] = "connect";
    msg["cookie"] = this->sessionCookie;

    // Setup a request
    QChannelsRequest* tcpRequest = buildRequest("control", msg, CM_RELIABLE);
    connect(tcpRequest, SIGNAL(reply(bool)), this, SLOT(handleTCPResponse(bool)));

    // Send the tcp request
    tcpRequest->send(false);
	qDebug("tcpConnected done");
} // end tcpConnected

void QChannels::handleLoginResponse(bool confirmed)
{
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    QChannelsRequest* loginReq = qobject_cast<QChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = loginReq->replyMessage;

    if(!confirmed)
    {
        qCritical() << "Server denied login: " << replyMessage["reason"].toString();
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
        this->tcpPort = replyMessage["tcpPort"].toUInt();
        this->udpPort = replyMessage["udpPot"].toUInt();

        this->sessionCookie = replyMessage["cookie"].toString();

        // Connect our TCP and UDP sockets.
        connectTransports();
    } // end if
} // end handleLoginResponse

void QChannels::handleTCPResponse(bool confirmed)
{
	qDebug("handleTCPResponse...");
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    QChannelsRequest* tcpReq = qobject_cast<QChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = tcpReq->replyMessage;

    if(!confirmed)
    {
        qCritical() << "Server denied tcp connection: " << replyMessage["reason"].toString();
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
		qDebug("Server approved TCP connection.");
        this->_tcpConnected = true;
        if(this->_udpConnected)
        {
            emit connected();
        } // end if
    } // end if
	qDebug("handleTCPResponse finished.");
} // end handleTCPResponse

void QChannels::handleUDPResponse(bool confirmed)
{
	qDebug("handleUDPResponse...");
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    QChannelsRequest* udpReq = qobject_cast<QChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = udpReq->replyMessage;

    if(!confirmed)
    {
        qCritical() << "Server denied udp connection: " << replyMessage["reason"].toString();
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
		qDebug("Server approved UDP connection.");
        this->_udpConnected = true;
        if(this->_tcpConnected)
        {
            emit connected();
        } // end if
    } // end if
	qDebug("handleUDPResponse finished.");
} // end handleUDPResponse

void QChannels::handleIncommingMessage(QByteArray data)
{
	QByteArray plainText = data;

	if(qobject_cast<QNetString*>(QObject::sender()) == this->tcpNetstring)
	{
		plainText = cipher->decrypt(data);
	} // end if

	QVariantMap envelope = QJsonDocument::fromJson(plainText).toVariant().toMap();

	qDebug() << "Data: " << QString(data);

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

    qCritical() << "Received incoming message with unknown type: " << envelope["type"];
} // end handleIncommingMessage

void QChannels::sslDataReady()
{
    sslNetstring->addData(sslSocket->readAll());
} // end sslDataReady

void QChannels::tcpDataReady()
{
    tcpNetstring->addData(tcpSocket->readAll());
} // end tcpDataReady

void QChannels::udpDataReady()
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

            // Now, parse as JSON, and emit.
            emit incommingMessage(QJsonDocument::fromJson(plainText).toVariant().toMap());
    } // end while
} // end udpDataReady


void QChannels::sslError(QAbstractSocket::SocketError error)
{
	qDebug() << "SSL Error: " << sslSocket->errorString();
	qDebug() << "SSL Error Code: " << error;
} // end sslDebug

void QChannels::tcpError(QAbstractSocket::SocketError error)
{
	qDebug() << "TCP Error: " << sslSocket->errorString();
} // end sslDebug
void QChannels::udpError(QAbstractSocket::SocketError error)
{
	qDebug() << "UDP Error: " << sslSocket->errorString();
} // end sslError

void QChannels::sslDisconnected()
{
	qDebug("SSL Disconnected.");
	disconnect();
} // end sslDisconnected

void QChannels::tcpDisconnected()
{
	qDebug("TCP Disconnected.");
	disconnect();
} // end sslDisconnected

void QChannels::udpDisconnected()
{
	qDebug("UDP Disconnected.");
	disconnect();
} // end sslDisconnected

/**********************************************************************************************************************/
