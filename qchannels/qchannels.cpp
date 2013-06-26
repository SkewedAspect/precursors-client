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
QChannels::QChannels(QObject *parent) : QObject(parent)
{
    sslNetstring = new QNetString();
    connect(sslNetstring, SIGNAL(dataReady(QString)), this, SLOT(handleIncommingMessage(QString)));

    tcpNetstring = new QNetString();
    connect(tcpNetstring, SIGNAL(dataReady(QString)), this, SLOT(handleIncommingMessage(QString)));

    // Create a new SSL socket, and conect it's signals.
    this->sslSocket = new QSslSocket(this);
    connect(this->sslSocket, SIGNAL(encrypted()), this, SLOT(sslConnected()));
    connect(this->sslSocket, SIGNAL(readyRead()), this, SLOT(sslDataReady()));
    //connect(this->sslSocket, SIGNAL(error()), this, SLOT(sslError()));
    //connect(this->sslSocket, SIGNAL(disconnected()), this, SLOT(sslDisconnected()));

    // Create a new TCP socket, and conect it's signals.
    this->tcpSocket = new QTcpSocket(this);
    connect(this->tcpSocket, SIGNAL(connected()), this, SLOT(tcpConnected()));
    connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpDataReady()));
    //connect(this->tcpSocket, SIGNAL(error()), this, SLOT(sslError()));
    //connect(this->tcpSocket, SIGNAL(disconnected()), this, SLOT(sslDisconnected()));

    // Create a new UDP socket, and conect it's signals.
    this->udpSocket = new QUdpSocket(this);
    connect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(udpDataReady()));
    //connect(this->udpSocket, SIGNAL(encrypted()), this, SLOT(udpConnected()));
    //connect(this->udpSocket, SIGNAL(error()), this, SLOT(udpError()));
    //connect(this->udpSocket, SIGNAL(disconnected()), this, SLOT(udpDisconnected()));
} // end QChannels

/**
 * @brief Connects to sever, on the given port.
 * @param serverHostName A string, representing the hostname of the server.
 * @param port The port to connect to the server on. (Note: this should be the ssl port.)
 */
void QChannels::connectToServer(QString serverHostName, quint16 port, QString username, QString pwdHash)
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
void QChannels::disconnect()
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
 * @brief QChannels::send
 * @param envelope
 * @param mode
 */
void QChannels::send(QVariant envelope, ChannelMode mode)
{
    QString jsonData = QJsonDocument::fromVariant(envelope).toJson();

    switch(mode)
    {
        case CM_SECURE:
        {
            sslSocket->write(QNetString::encode(jsonData).toUtf8());
            break;
        } // end CM_SECURE

        case CM_UNRELABLE:
        {
            udpSocket->writeDatagram(jsonData.toUtf8(), this->serverAddress, this->udpPort);
            break;
        } // end CM_UNRELIABLE

        case CM_RELIABLE:
        default:
        {
            tcpSocket->write(QNetString::encode(jsonData).toUtf8());
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
void QChannels::sendEvent(QString channel, QVariant message, ChannelMode mode)
{
    QVariantMap* envelope = wrapMessage(channel, "event", message);
    send(*envelope, mode);
} // end sendEvent

/**
 * @brief QChannels::sendRequest
 * @param request
 */
void QChannels::sendRequest(QChannelsRequest* request)
{
    QVariantMap* envelope = wrapMessage(request->channel, "request", request->requestMessage);
    send(*envelope, request->mode);
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
    request->replyMessage = envelope["content"].toMap();
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

} // end connectTransports

// Wraps the mesage in an envelope for sending
QVariantMap* QChannels::wrapMessage(QString channel, QString type, QVariant message)
{
    QVariantMap* envelope = new QVariantMap();
    envelope->insert("type", type);
    envelope->insert("channel", channel);
    envelope->insert("content", message);

    return envelope;
} // end wrapMessage

/**********************************************************************************************************************/
/* Private Slots                                                                                                      */
/**********************************************************************************************************************/

void QChannels::sslConnected()
{
    // Store the IP address of the server.
    this->serverAddress = this->sslSocket->peerAddress();

    // Buid our version list
    QList<QVariant>* version = new QList<QVariant>();
    version->append(0);						// Major
    version->append(5);						// Minor
    version->append(0);						// Micro
    version->append("Client PreAlpha 1");	// Release name

    // Build the login message
    QVariantMap* msg = new QVariantMap();
    msg->insert("type", "login");
    msg->insert("user", this->username);
    msg->insert("password", this->pwdHash);

    //TODO: We should get this information somehow. Perhaps from a header?
    msg->insert("version", *version);
    msg->insert("clientName", "Official Precursors Client");

    //TODO: We need to generate actual keys and vectors
    msg->insert("iv", "vector!!!#$%^");
    msg->insert("key", "super-secure-key!111##&*");

    // Setup a request
    QChannelsRequest* loginRequest = buildRequest("control", *msg, CM_SECURE);
    connect(loginRequest, SIGNAL(reply(bool)), this, SLOT(handleLoginResponse(bool)));

    // Send the login request
    loginRequest->send();
} // end sslConnected

void QChannels::tcpConnected()
{

} // end tcpConnected

void QChannels::handleLoginResponse(bool confirmed)
{
    //TODO: There might be reasons to avoid this... but it was easier, for now.
    QChannelsRequest* loginReq = qobject_cast<QChannelsRequest*>(QObject::sender());
    QVariantMap replyMessage = loginReq->replyMessage;

    if(!confirmed)
    {
        qCritical() << "Server dened login: " << replyMessage["reason"].toString();
        emit disconnected(replyMessage["reason"].toString());
    }
    else
    {
        this->tcpPort = replyMessage["tcpPort"].toUInt();
        this->udpPort = replyMessage["udpPot"].toUInt();

    } // end if
} // end handleLoginResponse

void QChannels::handleTCPResponse(bool confirmed)
{

} // end handleTCPResponse

void QChannels::handleUDPResponse(bool confirmed)
{

} // end handleUDPResponse

void QChannels::handleIncommingMessage(QString data)
{
    //TODO Decrypt from AES here
    QVariantMap envelope = QJsonDocument::fromJson(data.toUtf8()).toVariant().toMap();

    if(envelope["type"] == "reply")
    {
        handleReply(envelope);
    } // end if

    if(envelope["type"] == "event")
    {
        handleEvent(envelope);
    } // end if

    qCritical("Received incoming message withut type!!");
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

} // end udpDataReady

/**********************************************************************************************************************/
