#include "qnetstring.h"

/**********************************************************************************************************************/
/* Public API                                                                                                         */
/**********************************************************************************************************************/

QNetString::QNetString(QObject *parent) :
    QObject(parent)
{
    // Connect to our internal signals
    connect(this, SIGNAL(tryParse()), this, SLOT(onTryParse()));
} // end QNetString

QByteArray QNetString::encode(QByteArray data)
{
	return QByteArray::number(data.length(), 10).append(':').append(data).append(',');
} // end encode

/**********************************************************************************************************************/
/* Public Slots                                                                                                       */
/**********************************************************************************************************************/

void QNetString::addData(QByteArray data)
{
    this->buffer.append(data);
    emit tryParse();
} // end addData

/**********************************************************************************************************************/
/* Private Slots                                                                                                      */
/**********************************************************************************************************************/

void QNetString::onTryParse()
{
    int netstringLength = -1;

    while(true)
    {
        // If the buffer's empty or we don't have a colon, we can't parse.
        if(this->buffer.length() == 0 || !buffer.contains(':'))
        {
            // We don't have enough data to read
            return;
        } // end if

        int idx = buffer.indexOf(':');

        // Determine our length
        netstringLength = buffer.left(idx).toInt();

        if(buffer.length() < idx + netstringLength + 2)
        {
            return;
        } // end if

        if(buffer[netstringLength + idx + 1] != ',')
        {
            qCritical() << "Invalid netstring: " << QString(buffer);
        } // end if

        // Pull out our data
        QByteArray data = buffer.mid(idx + 1, netstringLength);

        //qDebug() << "Data: " << data;

        // Trim our buffer, and reset our lengh
        buffer = buffer.mid(idx + netstringLength + 2);

        //qDebug() << "Buffer: " << buffer;

        // Emit the data
        emit dataReady(data);
    } // end while
} // end onTryParse
