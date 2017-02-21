#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlListProperty>
#include "./Twitter/Twitter.h"
#include "./Model/model.h"
#include "./Exceptions/createFolderException.h"

#include <QDateTime>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QQmlEngine>
#include <klocale.h>


typedef enum TAB {TAB_TWEETS, TAB_NOTIFS, TAB_MSGS} TAB;

/// Main interface for comunication between frontend and backend.
/** Detailed description. */
class Backend : public QObject 
{
    Q_OBJECT
    Q_DISABLE_COPY(Backend)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loggChanged)
    Q_PROPERTY(QString AuthLink READ getAuthLink NOTIFY authLinkChanged)
    Q_PROPERTY(QString credInfo READ getTextCredInfo NOTIFY credInfoChanged)
    Q_PROPERTY(QObject* statusesModel READ statusesModel NOTIFY statusesModelChanged)
    Q_PROPERTY(QObject* notificationsModel READ notificationsModel NOTIFY notificationsModelChanged)
    Q_PROPERTY(QObject* conversationsModel READ conversationsModel NOTIFY conversationsModelChanged)
    Q_PROPERTY(QObject* conversationModel READ conversationModel NOTIFY conversationModelChanged)
    Q_PROPERTY(unsigned long long setConversationId WRITE setConversationId)    

    public:
        static Backend* instance();
        static void destroy();

        bool isLoggedIn() const;
        QString getAuthLink() const;
        QString getTextCredInfo() const;
        QObject* statusesModel() const;
        QObject* notificationsModel() const;
        QObject* conversationsModel() const;
        QObject* conversationModel() const;
        void setConversationId(unsigned long long id);

    public Q_SLOTS:
        void login(const QString & pin);
        void logout();
        void authenticate();
        void postNewStatus(const QString & status);
        void sendMessage(const QString & message);
        void resetNotifsIndicator();
        void resetMessagIndicator();
        void selectedTab(int num);
        void notificationsEnabled(bool enabled);
        void setConsumerCred(const QString & consumerKey, const QString & consumerSecret);
        
        void startStreamConsum();

    private slots:
        void latestNotifsChangeCallback(const QString&, const QString&);
        void latestMessagChangeCallback(const QString&, const QString&);

    signals:
        void loggChanged();
        void authLinkChanged();
        void credInfoChanged();
        void latestNotifsChanged(const QString& latestNotifsIndicator);
        void latestMessagChanged(const QString& latestMessagIndicator);
        
        QObject* statusesModelChanged(QObject*);
        void notificationsModelChanged();
        void conversationsModelChanged();
        void conversationModelChanged();

        
	private:
        Backend(QObject *parent = 0);
        ~Backend();
        
        static Backend* m_pInstance;

        void initModel();
        static void createFilesDirectory(const QString & path);
        static void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

        Twitter* m_twitter; //!< Object for communication with Twitter. 
        Model* m_model;     //!< Object that represents the data model.  
        QString m_imagePrefix;
        QString m_latestNotifsIndicator;    
        QString m_latestMsgIndicator;
        unsigned long long m_id;


        QSystemTrayIcon* systemTray;
        TAB m_selectedTab;    

        bool m_notifsEnabled;
};

#endif
