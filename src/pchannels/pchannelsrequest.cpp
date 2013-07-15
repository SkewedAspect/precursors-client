#include "pchannelsrequest.h"

/*********************************************************************************************************************/
/* Public API                                                                                                        */
/*********************************************************************************************************************/

/**
 * @brief A class to make working with request messages easier.
 * This class gives the calling code a single signal it needs to listen for, instead of listening for all replies and
 * seeing if the incoming message matters. It also contains both the reply and the original request message for reference.
 * @param parent A pointer to our PChannels instance.
 * @param channel The channel to send the request on.
 * @param message The body of the request.
 * @param mode The ChannelMode to send with.
 */
PChannelsRequest::PChannelsRequest(PChannels* parent, QString channel, QVariant message, PChannels::ChannelMode mode) :
    QObject(parent), _isOutstanding(true)
{
    this->parent = parent;
    this->channel = channel;
    this->requestMessage = message;
    this->mode = mode;
} // end PChannelsRequest


PChannelsRequest::PChannelsRequest() :
    QObject(), _isOutstanding(true)
{
    this->parent = 0;
} // end PChannelsRequest


/**
 * @brief Checks to see if the reply is still outstanding, or if it's been replied to.
 * @return Returns true if this request is still outstanding, or false if it's been replied to. Defaults to true.
 */
bool PChannelsRequest::isOutstanding()
{
    return this->_isOutstanding;
} // end isOutstanding

/**
 * @brief Sends this request to the server. (Note: this should be called *after* connecting to it's signals)
 */
void PChannelsRequest::send(bool encrypted)
{
    this->parent->sendRequest(this, encrypted);
} // end send

void PChannelsRequest::fireReply()
{
    bool _confirmed = this->replyMessage["confirm"].toBool();
    emit reply(_confirmed);

    if(_confirmed)
    {
        emit confirmed();
    }
    else
    {
        emit denied();
    } // end if
} // end fireReply

/*********************************************************************************************************************/
