#ifndef TWITTERWORKER_H
#define	TWITTERWORKER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <oauth.h>
#include <curl/curl.h>
#include <sstream>

#include "libtwitcurl/twitcurl.h"
#include "ImageDownloader.h"

#include <QObject>
#include <QString>
#include <QThread>
#include <QDebug>

using namespace std;

///Worker class for comunnication with Twitter
/** Sould working in non gui thread. Maintains a long connection to the server. */
class TwitterWorker : public QObject
{
    Q_OBJECT
public:
	TwitterWorker(         
        const string& ConsKey,
        const string& ConsSecret,
        const string& AccessTokenKey,
        const string& AccessTokenSecret,
        ImageDownloader* imgDownloader,
        twitCurl* tCurl);
    ~TwitterWorker();

    static size_t fncCallback(char* ptr, size_t size, size_t nmemb, void* f);
    void nonStaticFunction(const string& str);

public slots:
    void doWork();

signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void sig_work_twitterChanged(QString);
    void sig_work_timelineHome(QString);
    void sig_work_accountVerifyCred(QString);
    void sig_work_directMessages(QString);
    void sig_work_directMessagesSent(QString);
    void sig_work_finished();

private:
    void getTimelineHome();
    void getAccountVerifyCred();
    void getDirectMessages();
    void getDirectMessagesSent();

    const char* m_consKey;
    const char* m_consSec;
    const char* m_tokeKey;
    const char* m_tokeSec;
    const char* m_url = "https://userstream.twitter.com/1.1/user.json";
    CURL *curl;
    char *cSignedUrl;
    QString m_buffer;

    ImageDownloader* m_imgDownloader;
    twitCurl* m_tCurl;
};

#endif 
