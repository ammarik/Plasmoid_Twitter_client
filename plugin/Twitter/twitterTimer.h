#ifndef TWITTERTIMER_H
#define	TWITTERTIMER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <oauth.h>
#include <curl/curl.h>
#include <sstream>


#include <QThread>
#include <QString>
#include <QTimer>

using namespace std;

///Simple timer
class TwitterTimer : public QObject
{
    Q_OBJECT
public:
	TwitterTimer();
	~TwitterTimer();

signals:
    void sig_timeout();

private slots:
    void startTimer(int);
    void stopTimer();
    void timeout();

private:

    QTimer* m_timer;
};

#endif 