#include "Twitter.h"

Twitter::Twitter(QObject *parent) : QObject(parent)
{
    oAuthTokenKeyFile = oAuthTokenSecretFile = oAuthConsTokenKeyFile = oAuthConsTokenSecretFile = Config::instance().getFilesPath().toUtf8().constData();
    oAuthTokenKeyFile += "twitterClient_token_key.txt";
    oAuthTokenSecretFile += "twitterClient_token_secret.txt";
    oAuthConsTokenKeyFile += "twitterClient_consumerToken_key.txt";
    oAuthConsTokenSecretFile += "twitterClient_consumertoken_secret.txt";

    initConsumerCreds();
    //m_imgDownloader->clean();
    
    m_twitterWorker = NULL;
    thrd_worker = NULL;
    m_reconnectTimer = NULL;
    thrd_rconnectTimer = NULL;

    tcpIpReconnectInterval = 2500;
    tcpIpReconnectMax = 16000;
    httpReconnectIntervalBase = 5000;
    httpReconnectMax = 320000;
    http420ReconnectIntervalBase = 60000;

    reconnectInterval = tcpIpReconnectInterval;    

    initTwitter();
    m_imgDownloader = new ImageDownloader();
    initRequester();
    initWorker();

    state = STARTING;
}
    
//------------------------------------------------------------------------------//

Twitter::~Twitter()
{
    if(thrd_worker != NULL)
    {
        thrd_worker->requestInterruption();
        m_twitterWorker = NULL;
    }

    state = STOPPING;

    emit sig_twit_deleteRequester();
    m_twitterRequester = NULL;

    delete m_tCurl;
    m_tCurl = NULL;
    delete m_imgDownloader;
}

//------------------------------------------------------------------------------//

///Sets API (consument) credentials
/**
 * @param consumerKey - API(consument) key
 * @param consumerSecret - API(consument) secret
 */
void Twitter::setConsumerCred(const QString & consumerKey, const QString & consumerSecret)
{
    if((consumerKey != "") && (consumerSecret != ""))
    {
        oAuthConsumerKey = consumerKey.toUtf8().constData();
        oAuthConsumerSecret = consumerSecret.toUtf8().constData();
        
        std::ofstream oAuthConsumerKeyOut;
        std::ofstream oAuthConsumerSecretOut;

        oAuthConsumerKeyOut.open( oAuthConsTokenKeyFile );
        oAuthConsumerSecretOut.open( oAuthConsTokenSecretFile );

        oAuthConsumerKeyOut.clear();
        oAuthConsumerSecretOut.clear();

        oAuthConsumerKeyOut << oAuthConsumerKey;
        oAuthConsumerSecretOut << oAuthConsumerSecret;

        oAuthConsumerKeyOut.close();
        oAuthConsumerSecretOut.close();

        if (!oAuthConsumerKeyOut) 
            throw MyFileException(oAuthConsTokenKeyFile);
        if(!oAuthConsumerSecretOut)
            throw MyFileException(oAuthConsTokenSecretFile);

        if(!QFile(QString::fromStdString(oAuthConsTokenKeyFile)).setPermissions(QFile::ReadOwner|QFile::WriteOwner))
        {
            oAuthConsumerKeyOut.open(oAuthConsTokenKeyFile);
            oAuthConsumerKeyOut.clear();
            oAuthConsumerKeyOut.close();
            throw PermFileException(oAuthConsTokenKeyFile);
        }
        if(!QFile(QString::fromStdString(oAuthConsTokenSecretFile)).setPermissions(QFile::ReadOwner|QFile::WriteOwner))
        {
            oAuthConsumerSecretOut.open(oAuthConsTokenSecretFile);
            oAuthConsumerSecretOut.clear();
            oAuthConsumerSecretOut.close();
            throw PermFileException(oAuthConsTokenSecretFile);
        }

        m_tCurl->getOAuth().setConsumerKey(oAuthConsumerKey);
        m_tCurl->getOAuth().setConsumerSecret(oAuthConsumerSecret);
    }
}

//------------------------------------------------------------------------------//

///Inits TwitCurl object
void Twitter::initTwitter()
{
    m_tCurl = new PlasTwitCurl;
    m_tCurl->getOAuth().setConsumerKey(oAuthConsumerKey);
    m_tCurl->getOAuth().setConsumerSecret(oAuthConsumerSecret);

    curl_easy_setopt(m_tCurl->getCurl(), CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(m_tCurl->getCurl(), CURLOPT_LOW_SPEED_LIMIT, 1);
    curl_easy_setopt(m_tCurl->getCurl(), CURLOPT_LOW_SPEED_TIME, 30);
}

//------------------------------------------------------------------------------//

void Twitter::initWorker()
{
}

//------------------------------------------------------------------------------//

///Inits, sets conections(signals and slots) and moves TwitterRequester to another thread 
void Twitter::initRequester()
{
    thrd_requester = new QThread();
    m_twitterRequester = new TwitterRequester(m_imgDownloader, m_tCurl);

    m_twitterRequester->moveToThread(thrd_requester);

    connect(this, SIGNAL(sig_twit_postNewStatus(const QString&)), m_twitterRequester, SLOT(postNewStatus(QString)));
    connect(this, SIGNAL(sig_twit_sendMessage(const QString&, const QString&)),  m_twitterRequester, SLOT(sendMessage(const QString&, const QString&)));
    connect(this, SIGNAL(sig_twit_downloadProfileImage(const QString&, const QString&, bool)), m_twitterRequester, SLOT(downloadProfileImage(const QString&, const QString&, bool)));
    //connect(thrd_requester, SIGNAL (started()), m_twitterRequester, SLOT (start()));
    connect(this, SIGNAL(sig_twit_deleteRequester()), m_twitterRequester, SLOT(destroyRequester()));
    connect(m_twitterRequester, SIGNAL (sig_req_finished()), thrd_requester, SLOT (quit()));
    connect(m_twitterRequester, SIGNAL (sig_req_finished()), m_twitterRequester, SLOT (deleteLater()));
    connect(thrd_requester, SIGNAL (finished()), thrd_requester, SLOT (deleteLater()));

    thrd_requester->start();
}

//------------------------------------------------------------------------------//

///Inits API (consument) credentials
void Twitter::initConsumerCreds()
{
    if(existsFile(oAuthConsTokenKeyFile) && existsFile(oAuthConsTokenSecretFile))
    {
        ifstream oAuthTokenKeyIn;
        ifstream oAuthTokenSecretIn;

        oAuthTokenKeyIn.open( oAuthConsTokenKeyFile );
        oAuthTokenSecretIn.open( oAuthConsTokenSecretFile );
        getline(oAuthTokenKeyIn, oAuthConsumerKey);
        getline(oAuthTokenSecretIn, oAuthConsumerSecret);
        oAuthTokenKeyIn.close();
        oAuthTokenSecretIn.close();
    }
    else
    {
        oAuthConsumerKey = "xWoKi6cbcvmDO5vANSEps4k5q";
        oAuthConsumerSecret = "byMvrHDibR5SmYFLEvMLOWHx2WbyAocm2TUsngGcCYcHQE3RM7";  
    }
}

//------------------------------------------------------------------------------//

///User login to twitter via pin.
/**
 * @param  pin - PIN code acquired from Twitter authentication page
 */
int Twitter::login(const string& pin)
{
    m_tCurl->getOAuth().setOAuthPin( pin );
    m_tCurl->oAuthAccessToken();
    
    m_tCurl->getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
    m_tCurl->getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );
    
    std::ofstream oAuthTokenKeyOut;
    std::ofstream oAuthTokenSecretOut;

    oAuthTokenKeyOut.open( oAuthTokenKeyFile );
    oAuthTokenSecretOut.open( oAuthTokenSecretFile );

    oAuthTokenKeyOut.clear();
    oAuthTokenSecretOut.clear();

    oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
    oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

    oAuthTokenKeyOut.close();
    oAuthTokenSecretOut.close();

    if (!oAuthTokenKeyOut) 
        throw MyFileException(oAuthTokenKeyFile);
    if(!oAuthTokenSecretOut)
        throw MyFileException(oAuthTokenSecretFile);

    if(!QFile(QString::fromStdString(oAuthTokenKeyFile)).setPermissions(QFile::ReadOwner|QFile::WriteOwner))
    {
        oAuthTokenKeyOut.open(oAuthTokenKeyFile);
        oAuthTokenKeyOut.clear();
        oAuthTokenKeyOut.close();
        throw PermFileException(oAuthTokenKeyFile);
    }
    if(!QFile(QString::fromStdString(oAuthTokenSecretFile)).setPermissions(QFile::ReadOwner|QFile::WriteOwner))
    {
        oAuthTokenSecretOut.open(oAuthTokenSecretFile);
        oAuthTokenSecretOut.clear();
        oAuthTokenSecretOut.close();
        throw PermFileException(oAuthTokenSecretFile);
    }


    if(state != STARTING)
    {
        state = STARTING;
        startStream();
    }
    return 0;
}

//------------------------------------------------------------------------------//

///Log out user from Twitter.
int Twitter::logout()
{
    std::ofstream oAuthTokenKeyOut;
    std::ofstream oAuthTokenSecretOut;

    oAuthTokenKeyOut.open( oAuthTokenKeyFile );
    oAuthTokenSecretOut.open( oAuthTokenSecretFile );

    oAuthTokenKeyOut.clear();
    oAuthTokenSecretOut.clear();

    oAuthTokenKeyOut.close();
    oAuthTokenSecretOut.close();

    myOAuthAccessTokenKey = "";
    myOAuthAccessTokenSecret = "";

    if(thrd_worker != NULL)
    {
        thrd_worker->requestInterruption();
        m_twitterWorker = NULL;
    }

    state = STOPPING;

    emit sig_twit_deleteRequester();
    m_twitterRequester = NULL;

    delete m_tCurl;
    m_tCurl = NULL;

    return 0;
}

//------------------------------------------------------------------------------//

///Autheticates logged in user.
int Twitter::authenticate()
{
    if(isLoggedIn())
    {
        m_tCurl->getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
        m_tCurl->getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);
        return 0;
    }
    return 1;
}

//------------------------------------------------------------------------------//

/// Returns information whether the user is logged in.
/**
 * @return ture - returns true if the user is logged in.
 * @return false - returns false if the user is not logged in.
 */
bool Twitter::isLoggedIn()
{
    ifstream oAuthTokenKeyIn;
    ifstream oAuthTokenSecretIn;
    oAuthTokenKeyIn.open( oAuthTokenKeyFile );
    oAuthTokenSecretIn.open( oAuthTokenSecretFile );
    getline(oAuthTokenKeyIn, myOAuthAccessTokenKey);
    getline(oAuthTokenSecretIn, myOAuthAccessTokenSecret);
    oAuthTokenKeyIn.close();
    oAuthTokenSecretIn.close();

    if(myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size())
         return true;
     else
         return false;  
}

//------------------------------------------------------------------------------//

///Returns link to page where the user acquires PIN in order to login.
/**
 * @return the authorization link
 */
string Twitter::getWebAuthorizeLink()
{
    initConsumerCreds();
    if(oAuthConsumerKey != "" && oAuthConsumerSecret != "")
    {
    }
    else
    {
         return "";
    }

    if(m_tCurl == NULL)
    {
        initTwitter();
    }

    string authUrl = "";
    m_tCurl->oAuthRequestToken(authUrl);
    return authUrl;
}

//------------------------------------------------------------------------------//

///Posts new tweet(status)
/**
 * @param  status text of the tweet
 */
int Twitter::postNewStatus(const QString& status)
{
    if(state == RUNNING)
    {
        emit sig_twit_postNewStatus(status);
    }
    return 0;
}

//------------------------------------------------------------------------------//

///Sends new direct message.
/**
 * Sends new direct message to user with qiven userId.
 * @param  message - text of the direct message
 * @param  userId - id of the recipient
 */
int Twitter::sendMessage(const QString& message, unsigned long long userId)
{
    if(state == RUNNING)
    {
        emit sig_twit_sendMessage(message, QString::number(userId));
    }
    return 0;
}

//------------------------------------------------------------------------------//

///Downloads profile image.
/**
 * Downloads profile image with given url to file with given name.
 * @param fileName - name of the file where the image will be saved.
 * @param imageUrl - url from where the image is downloaded.
 */
void Twitter::downloadProfileImage(const QString & fileName, const QString & imageUrl)
{
    emit sig_twit_downloadProfileImage(fileName, imageUrl, false);
}

//------------------------------------------------------------------------------//

///Starts subscribing stream.
void Twitter::startStream()
{  
    if(isLoggedIn())
    { 
        if(m_twitterRequester == NULL)
        {
            initRequester();
        }

        if(m_twitterWorker == NULL)
        {
            thrd_worker = new QThread();

            m_twitterWorker = new TwitterWorker(
                oAuthConsumerKey,
                oAuthConsumerSecret, 
                myOAuthAccessTokenKey, 
                myOAuthAccessTokenSecret,
                m_imgDownloader,
                m_tCurl);

            m_twitterWorker->moveToThread(thrd_worker);

            connect(m_twitterWorker, SIGNAL(sig_work_timelineHome(QString)), this, SLOT(twitTimelineHomeCallback(QString)));
            connect(m_twitterWorker, SIGNAL(sig_work_accountVerifyCred(QString)), this, SLOT(twitAccountVerifyCredCallback(QString)));
            connect(m_twitterWorker, SIGNAL(sig_work_directMessages(QString)), this, SLOT(twitDirectMessagesCallback(QString)));
            connect(m_twitterWorker, SIGNAL(sig_work_directMessagesSent(QString)), this, SLOT(twitDirectMessagesSentCallback(QString)));
            connect(m_twitterWorker, SIGNAL(sig_work_twitterChanged(QString)), this, SLOT(twitTwitterChangeCallback(QString)));

            connect(thrd_worker, SIGNAL (started()), m_twitterWorker, SLOT (doWork()));
            connect(m_twitterWorker, SIGNAL (sig_work_finished()), this, SLOT(twitTimeout()));
            connect(m_twitterWorker, SIGNAL (sig_work_finished()), thrd_worker, SLOT (quit()));
            connect(m_twitterWorker, SIGNAL (sig_work_finished()), m_twitterWorker, SLOT (deleteLater()));
            connect(thrd_worker, SIGNAL (finished()), thrd_worker, SLOT (deleteLater()));
            thrd_worker->start();
        }
    }
}

//------------------------------------------------------------------------------//

///This should be called if the streming API timeouts.
/**
 * Starts reconnection procedure.
 */
void Twitter::twitTimeout()
{
    if(state == RUNNING)
    {
        m_twitterWorker = NULL;
        thrd_rconnectTimer = new QThread();
        m_reconnectTimer = new TwitterTimer();

        m_reconnectTimer->moveToThread(thrd_rconnectTimer);

        connect(this, SIGNAL(sig_twit_startReconnectTimer(int)), m_reconnectTimer, SLOT(startTimer(int)));
        connect(m_reconnectTimer, SIGNAL(sig_timeout()), this, SLOT(reconnectTimeout()));
        connect(m_reconnectTimer, SIGNAL (sig_timeout()), thrd_rconnectTimer, SLOT (quit()));
        connect(m_reconnectTimer, SIGNAL (sig_timeout()), m_reconnectTimer, SLOT (deleteLater()));
        connect(thrd_rconnectTimer, SIGNAL (finished()), thrd_rconnectTimer, SLOT (deleteLater()));
    
        thrd_rconnectTimer->start();

        state = WAITING;

        emit sig_twit_startReconnectTimer(reconnectInterval);
    }
}

//------------------------------------------------------------------------------//

///Reconnect timeout handler
/**
 * Trying to reconnect in loop with interval specified by Twitter documentation. 
 */
void Twitter::reconnectTimeout()
{   
    ERROR err = TCP_IP_ERR;

    switch(err)
    {
        case TCP_IP_ERR:
            if(reconnectInterval < tcpIpReconnectMax)
                reconnectInterval += tcpIpReconnectInterval;
            startStream();
            break;
        case HTTP_ERR:
            if(reconnectInterval < httpReconnectMax)
            {
                if(reconnectInterval < httpReconnectIntervalBase)
                    reconnectInterval = httpReconnectIntervalBase;
                else
                    reconnectInterval = 2 * reconnectInterval;
            }
            startStream();
            break;
        case HTTP420_ERR:
            if(reconnectInterval < http420ReconnectIntervalBase)
                    reconnectInterval = http420ReconnectIntervalBase;
                else
                    reconnectInterval = 2 * reconnectInterval;
            startStream();
        default:
            break;
    }
}

//------------------------------------------------------------------------------//

void Twitter::twitTwitterChangeCallback(QString data)
{
    if(state != STOPPING)
    {
        state = RUNNING;
        emit sig_twit_twitterChanged(data);
    }
}

//------------------------------------------------------------------------------//

void Twitter::twitTimelineHomeCallback(QString data)
{
    if(data != "")
    {
        emit sig_twit_reconnect();
        emit sig_twit_timelineHome(data);
    }
}

//------------------------------------------------------------------------------//

void Twitter::twitAccountVerifyCredCallback(QString data)
{
    emit sig_twit_accountVerifyCred(data);
}

//------------------------------------------------------------------------------//

void Twitter::twitDirectMessagesCallback(QString data)
{
    emit sig_twit_directMessages(data);
}

//------------------------------------------------------------------------------//

void Twitter::twitDirectMessagesSentCallback(QString data)
{
    emit sig_twit_directMessagesSent(data);
}

//------------------------------------------------------------------------------//

///Returns information if the given file exists.
/**
 * @param  name - path of the file.
 * @return true - if the file given by path in argument exists.
 * @return false - if the file given by path in argument not exists.
 */
bool Twitter::existsFile (const std::string& name) 
{
    if (FILE *file = fopen(name.c_str(), "r")) 
    {
        fclose(file);
        return true;
    }
    else 
    {
        return false;
    }   
}
