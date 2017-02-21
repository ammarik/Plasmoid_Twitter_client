#include "twitterRequester.h"

//------------------------------------------------------------------------------//

TwitterRequester::TwitterRequester(
    ImageDownloader* imgDownloader,
    twitCurl* tCurl) : m_imgDownloader(imgDownloader), m_tCurl(tCurl)
{
    
} 

//------------------------------------------------------------------------------//

TwitterRequester::~TwitterRequester()
{
}

//------------------------------------------------------------------------------//

void TwitterRequester::destroyRequester()
{
    emit sig_req_finished();
}

//------------------------------------------------------------------------------//

///Posts new tweet(status)
/**
 * @param  status text of the tweet
 */
int TwitterRequester::postNewStatus(QString status)
{
    string replyMsg;
    replyMsg = "";

    string status_utf8 = status.toUtf8().constData();
    
    if( m_tCurl->statusUpdate( status_utf8 ) )
    {
        m_tCurl->getLastWebResponse( replyMsg );
        return 0;
    }
    else
    {
        m_tCurl->getLastCurlError( replyMsg );
        return -1;
    }
}

//------------------------------------------------------------------------------//

///Sends new direct message.
/**
 * Sends new direct message to user with qiven userId.
 * @param  message - text of the direct message
 * @param  userId - id of the recipient
 */
int TwitterRequester::sendMessage(const QString& message, const QString& userId)
{
    string replyMsg;
    replyMsg = "";

    string message_utf8 = message.toUtf8().constData();
    string userId_utf8 = userId.toUtf8().constData();

    if( m_tCurl->directMessageSend(userId_utf8, message_utf8, true) )
    {
        m_tCurl->getLastWebResponse( replyMsg );
        return 0;
    }
    else
    {
        m_tCurl->getLastCurlError( replyMsg );
        return -1;
    }
}

//------------------------------------------------------------------------------//

///Stars downloading of image.
/**
 * @param fileName  - name of the output image file
 * @param imageUrl  - url from where the image is downloaded.
 * @param overwrite - if false then - if exists file with same name as in param then file will not be downloaded.
 * @param overwrite - old files will be overwriten.
 */
void TwitterRequester::downloadProfileImage(const QString & fileName, const QString & imageUrl, bool overwrite)
{
    m_imgDownloader->startDownload(fileName.toUtf8().constData(), imageUrl.toUtf8().constData(), overwrite);    
}

