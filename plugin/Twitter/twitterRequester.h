#ifndef TWITTERREQUESTER_H
#define	TWITTERREQUESTER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <oauth.h>
#include <curl/curl.h>
#include <sstream>

#include "ImageDownloader.h"
#include "libtwitcurl/twitcurl.h"

#include <QObject>
#include <QString>

using namespace std;

/// Provides simple requests to server. Should be runned in non-gui thread.
class TwitterRequester : public QObject
{
    Q_OBJECT
public:
	TwitterRequester(
        ImageDownloader* imgDownloader,         
        twitCurl* tCurl);

    ~TwitterRequester();


signals:
    void sig_req_finished();

public slots:
    void destroyRequester();
    int postNewStatus(QString status);
    int sendMessage(const QString& message, const QString& userId);
    void downloadProfileImage(const QString & fileName, const QString & imageUrl, bool overwrite);

private:
    ImageDownloader* m_imgDownloader;
    twitCurl* m_tCurl;
};

#endif 
