#include "TwitterWorker.h"

//------------------------------------------------------------------------------//

///Creates new worker
/**
 * @param ConsumerKey - API(Consumer) key
 * @param ConsumerSecret - API(Consumer) key
 * @param AccessTokenKey - Users key
 * @param AccessTokenSecret - Users secret
 * @param imgDownloader - image downloader
 * @param tCurl - instance of twitcurl
 */
TwitterWorker::TwitterWorker(
    const string& ConsumerKey,
    const string& ConsumerSecret, 
    const string& AccessTokenKey, 
    const string& AccessTokenSecret,
    ImageDownloader* imgDownloader,
    twitCurl* tCurl) : m_imgDownloader(imgDownloader), m_tCurl(tCurl)
{
    m_buffer = "";

    m_consKey = ConsumerKey.c_str();
    m_consSec = ConsumerSecret.c_str();
    m_tokeKey = AccessTokenKey.c_str();
    m_tokeSec = AccessTokenSecret.c_str();
}

//------------------------------------------------------------------------------//

TwitterWorker::~TwitterWorker()
{
}  

//------------------------------------------------------------------------------//

/// Main worker method
/**
 * Gets initial tweets, verify data, received messages, sent messages. Then instatiates and maintains connection to Streaming API.  
 */
void TwitterWorker::doWork()
{
    getTimelineHome();
    getAccountVerifyCred();
    getDirectMessages();
    getDirectMessagesSent();

    curl = curl_easy_init();
    if (!curl)
    {
        curl_global_cleanup();
        return;
    }

    cSignedUrl = oauth_sign_url2(m_url, NULL, OA_HMAC, "GET", m_consKey, m_consSec, m_tokeKey, m_tokeSec);

    // - URL
    curl_easy_setopt(curl, CURLOPT_URL, cSignedUrl);
    // - User agent name
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "mk-mode BOT");
    // - HTTP STATUS >=400 ---> ERROR
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    // - Callback function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fncCallback);
    // - Write data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 90);

    // ==== Execute
    int iStatus = curl_easy_perform(curl);
    if (!iStatus)
       qCritical() << "[ERROR] curl_easy_perform: STATUS=" << iStatus;

    // ==== cURL Cleanup
    curl_easy_cleanup(curl);  

    emit sig_work_finished();
}

//------------------------------------------------------------------------------//

///Satic callback called from doWork() method if there is change received from streaming API.
size_t TwitterWorker::fncCallback(char* ptr, size_t size, size_t nmemb,  void *f) 
{
    int iRealSize = size * nmemb;

    if(QThread::currentThread()->isInterruptionRequested())
    {
        // ==== cURL Cleanup
        //curl_easy_cleanup(curl);  
        //emit sig_work_finished();
        return -1;
    }

    //Call non-static member function. 
    static_cast<TwitterWorker*>(f)->nonStaticFunction(string(ptr, iRealSize)); 

    return iRealSize;
}

//------------------------------------------------------------------------------//

///Buffered NON-Satic callback called from doWork() method via fncCallback method if there is change received from API.
/**
 * Emits signal with string that represents the change. 
 * @param str - string received from server, will be appended to buffer,  buffer will be emited if there is full length message
 */
void TwitterWorker::nonStaticFunction(const string& str)
{
    m_buffer.append(QString::fromStdString(str));

    if(m_buffer.endsWith("\r\n"))
    {
        emit sig_work_twitterChanged(m_buffer);
        m_buffer = "";
    }
}

//-----------------------------------------------------------------------------//

/// Gets latest tweets.
void TwitterWorker::getTimelineHome()
{
    string replyMsg, errorMsg;
    replyMsg = "";
    
    if( m_tCurl->timelineHomeGet() )
    {
        m_tCurl->getLastWebResponse( replyMsg );        
    }
    QString qstr = QString::fromStdString(replyMsg);

    emit sig_work_timelineHome(qstr);
}

//------------------------------------------------------------------------------//

/// Gets verify credentials.
void TwitterWorker::getAccountVerifyCred()
{
    string replyMsg;
    replyMsg = "";

    if( m_tCurl->accountVerifyCredGet() )
    {
        m_tCurl->getLastWebResponse( replyMsg );
    }

    QString qstr = QString::fromStdString(replyMsg);

    emit sig_work_accountVerifyCred(qstr);
}

//------------------------------------------------------------------------------//

/// Gets latest received direct messages.
void TwitterWorker::getDirectMessages()
{
    string replyMsg  = "";

    if( m_tCurl->directMessageGet() )
    {
        m_tCurl->getLastWebResponse( replyMsg );
    }

    QString qstr = QString::fromStdString(replyMsg);

    emit sig_work_directMessages(qstr);
}

//------------------------------------------------------------------------------//

/// Gets latest sent direct messages.
void TwitterWorker::getDirectMessagesSent()
{
    string replyMsg = "";

    if( m_tCurl->directMessageGetSent() )
    {
        m_tCurl->getLastWebResponse( replyMsg );
    }

    QString qstr = QString::fromStdString(replyMsg);

    emit sig_work_directMessagesSent(qstr);
}

//------------------------------------------------------------------------------//
