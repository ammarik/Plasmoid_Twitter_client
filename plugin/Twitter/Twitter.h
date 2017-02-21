#ifndef TWITTER_H
#define	TWITTER_H

#include <cstdlib>
#include <cstring>
//#include <QNetworkAccessManager>
#include "plasTwitCurl.h"
#include "TwitterWorker.h"
#include "twitterTimer.h"
#include "twitterRequester.h"
#include "ImageDownloader.h"
#include "../config.h"
#include "../Exceptions/myFileException.h"
#include "../Exceptions/permFileException.h"

using namespace std;

typedef enum STATE { STARTING, RUNNING, WAITING, STOPPING } STATE;
typedef enum ERROR { TCP_IP_ERR, HTTP_ERR, HTTP420_ERR } ERROR;

///Provides connection to twitter server
class Twitter : public QObject
{
    Q_OBJECT
public:
    Twitter(QObject *parent = 0);
    virtual ~Twitter();
    
    bool isLoggedIn();
    string getWebAuthorizeLink();
    int login(const string& pin);
    int logout();
    int authenticate();
    void setConsumerCred(const QString & consumerKey, const QString & consumerSecret);

    void startStream();
    
    int postNewStatus(const QString& status);
    int sendMessage(const QString& message, unsigned long long userId);
    void downloadProfileImage(const QString & fileName, const QString & imageUrl);

private slots:
    void twitTwitterChangeCallback(QString data);
    void twitTimelineHomeCallback(QString data);
    void twitAccountVerifyCredCallback(QString data);
    void twitDirectMessagesCallback(QString data);
    void twitDirectMessagesSentCallback(QString data);

    void twitTimeout();
    void reconnectTimeout();

signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void sig_twit_twitterChanged(QString);
    void sig_twit_timelineHome(QString);
    void sig_twit_accountVerifyCred(QString);
    void sig_twit_directMessages(QString);
    void sig_twit_directMessagesSent(QString);
    void sig_twit_reconnect();

    void sig_twit_postNewStatus(const QString&);
    void sig_twit_sendMessage(const QString&, const QString&);
    void sig_twit_downloadProfileImage(const QString&, const QString&, bool);
    void sig_twit_startReconnectTimer(int);

    void sig_twit_deleteRequester();

private:

    void initTwitter();
    void initWorker();
    void initRequester();
    void initConsumerCreds();

    bool existsFile(const std::string& name);

    PlasTwitCurl* m_tCurl;
    string oAuthConsumerKey;
    string oAuthConsumerSecret;
    string myOAuthAccessTokenKey;
    string myOAuthAccessTokenSecret;
    
    string oAuthTokenKeyFile;
    string oAuthTokenSecretFile;
    string oAuthConsTokenKeyFile;
    string oAuthConsTokenSecretFile;

    TwitterTimer* m_reconnectTimer;
    TwitterWorker* m_twitterWorker;
    TwitterRequester* m_twitterRequester;
    ImageDownloader* m_imgDownloader;

    QThread* thrd_rconnectTimer;
    QThread* thrd_worker;
    QThread* thrd_requester;

    int tcpIpReconnectInterval;
    int tcpIpReconnectMax;
    int httpReconnectIntervalBase;
    int httpReconnectMax;
    int http420ReconnectIntervalBase;

    int reconnectInterval;

    STATE state;
};

#endif	/* TWITTER_H */

