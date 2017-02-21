 #include "backend.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QQuickView>
#include "./Twitter/Twitter.h"

Backend* Backend::m_pInstance = NULL; 

/// Brief description.
/** Detailed description. */
Backend::Backend(QObject *parent) : QObject(parent) 
{
    createFilesDirectory(Config::instance().getFilesPath());
    createFilesDirectory(Config::instance().getImagesPath());
 
    qInstallMessageHandler(logMessageOutput);

	QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    m_twitter = new Twitter(parent);
    m_model = NULL;

    systemTray = new QSystemTrayIcon(this);
	systemTray->setVisible(true);

    m_id = 0;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    m_selectedTab = TAB_TWEETS;
    m_notifsEnabled = true;
}

//------------------------------------------------------------------------------//

/// Creates singleton instance of Backend.
/**
 * @return pointer to instance of Backed.
 */
Backend* Backend::instance()
{
	if(!m_pInstance)
	{
		m_pInstance = new Backend();
	}

	return m_pInstance;
}

//------------------------------------------------------------------------------//

Backend::~Backend() 
{
	delete m_twitter;
	if(m_model != NULL)
	{
		delete m_model; 
	}
}

//------------------------------------------------------------------------------//

/// Destroys backend.
void Backend::destroy()
{
	delete m_pInstance;
}

//------------------------------------------------------------------------------//

/// Initialize model
void Backend::initModel()
{
	m_model = new Model(m_twitter);
	m_latestNotifsIndicator = " ";
	connect(m_model, SIGNAL(latestNotifsModelChanged(const QString&, const QString&)), this, SLOT(latestNotifsChangeCallback(const QString&, const QString&)));
    connect(m_model, SIGNAL(latestMsgsModelChanged(const QString&, const QString&)), this, SLOT(latestMessagChangeCallback(const QString&, const QString&)));
    emit statusesModelChanged(m_model->getStatusesModel());
    emit notificationsModelChanged();
    emit conversationsModelChanged();
    emit conversationModelChanged();
}

//------------------------------------------------------------------------------//

void Backend::createFilesDirectory(const QString & path)
{
	if(!QDir().mkpath(path))
		throw CreateFolderException(path);
}

//------------------------------------------------------------------------------//

/// Creates new Qt message handler
/**
 * @param type    
 * @param context 
 * @param msg     
 */
void Backend::logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

	QString filesPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);	
	filesPath += "/plasmoids/cz.cvut.fit.marikma5/log";
	createFilesDirectory(filesPath);

	filesPath += "/twitterPlasmoid_log.txt";
    
    QFile file(filesPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream out(&file);

    switch (type) 
    {
    	case QtDebugMsg:
    		out << "Debug: " << localMsg.constData() << " (" << context.file << ", " << context.line << ", "<< context.function << ")\n";
    	    break;
    	case QtInfoMsg:
    	    out << "Info: " << localMsg.constData() << " (" << context.file << ", " << context.line << ", "<< context.function << ")\n";
    	    break;
    	case QtWarningMsg:
    	    out << "Warning: " << localMsg.constData() << " (" << context.file << ", " << context.line << ", "<< context.function << ")\n";
    	    break;
    	case QtCriticalMsg:
    	    out << "Critical: " << localMsg.constData() << " (" << context.file << ", " << context.line << ", "<< context.function << ")\n";
    	    break;
    	case QtFatalMsg:
    	    out << "Fatal: " << localMsg.constData() << " (" << context.file << ", " << context.line << ", "<< context.function << ")\n";
    	    abort();
    }

    file.flush();
    file.close();
}


//------------------------------------------------------------------------------//

/// Return true if the usere is logged in.
/** @return True if logged in false if not. */
bool Backend::isLoggedIn() const
{
    return m_twitter->isLoggedIn();
}

//------------------------------------------------------------------------------//

/// Get http link to autorization website.
/** @return The authorization link. */
QString Backend::getAuthLink() const
{
	string s = m_twitter->getWebAuthorizeLink();
	string link;
	
	if(s != "")
	{
		link = "<a href='"; 
		link.append(s);
		link.append("'>").append(i18n("Click to get PIN").toUtf8().constData()).append(".</a>");
	}
	else
	{
		link = i18n("Please register app (Set API credentials)").toUtf8().constData();
	}

	QString qstr = QString::fromStdString(link);
	return qstr;
}

//------------------------------------------------------------------------------//

QString Backend::getTextCredInfo() const
{
	return "Optional, but recommended to change API credentials!";
}

//------------------------------------------------------------------------------//

/// Returns model of list of statuses(tweets).
/** @return StatusesModel */
QObject* Backend::statusesModel() const
{
	if(m_model != NULL)
		return m_model->getStatusesModel();

	return NULL;
}

//------------------------------------------------------------------------------//

/// Returns model of list of notifications.
/** @return NotificationsModel */
QObject* Backend::notificationsModel() const
{
	if(m_model != NULL)
		return m_model->getNotificationsModel();
	return NULL;
}

//------------------------------------------------------------------------------//

/// Returns model of list of conversations.
/** @return ConversationsModel */
QObject* Backend::conversationsModel() const
{
	if(m_model != NULL)
		return m_model->getDirectMessangingModel();
	return NULL;
}

//------------------------------------------------------------------------------//

/// Returns model of conversation i.e model of list of messages.
/** @return ConversationsModel */
QObject* Backend::conversationModel() const
{
	if(m_id != 0)
	{
		if(m_model->getConversationModel(m_id) != NULL)
			return m_model->getConversationModel(m_id);
	}
	return NULL;
}

//------------------------------------------------------------------------------//

/// Set id of actual conversation.
/**
 * @param id of actual conversation.
 */
void Backend::setConversationId(unsigned long long id)
{
	m_id = id;
}

//------------------------------------------------------------------------------//

/// Login user 
/**
 * @param pin code used to login
 */
void Backend::login(const QString & pin)
{
	if(!m_twitter->isLoggedIn())
	{
		string utf8_pin = pin.toUtf8().constData();
		m_twitter->login(utf8_pin);
	}
}

//------------------------------------------------------------------------------//

/// Logout user
void Backend::logout()
{
	if(m_twitter->isLoggedIn())
	{
		m_twitter->logout();
		delete m_model;
		m_model = NULL;
	}
}


//------------------------------------------------------------------------------//

/// Authentication of the logged-in user.
void Backend::authenticate()
{
	if (m_twitter->isLoggedIn())
	{
		m_twitter->authenticate();	
	}
}

//------------------------------------------------------------------------------//

/// Posts new status(tweet).
/**
 * @param status - text of the new status(tweet)
 */
void Backend::postNewStatus(const QString & status)
{
	if(m_twitter->isLoggedIn())
	{
		authenticate();
		m_twitter->postNewStatus(status);
	}
}

//------------------------------------------------------------------------------//

/// Send a new message.
/**
 * @param message - text of the new message.
 */
void Backend::sendMessage(const QString & message)
{
	if(m_twitter->isLoggedIn())
	{
		authenticate();
		m_twitter->sendMessage(message, m_id);
	}
}

//------------------------------------------------------------------------------//

/// Starts consuming twitter stream.
void Backend::startStreamConsum()
{
	if(m_twitter->isLoggedIn())
	{	
		if (m_model == NULL)
		{
			initModel();
		}

		authenticate();
		m_model->initAll();
	}
}

//------------------------------------------------------------------------------//

/// Sets the currently selected tab.
/**
 * @param tabNum - nuber of the selected tab.
 */
void Backend::selectedTab(int tabNum)
{
	switch(tabNum)
	{
		case TAB_TWEETS:
		{
			m_selectedTab = TAB_TWEETS;
			break;
		}
		case TAB_NOTIFS:
		{
			m_selectedTab = TAB_NOTIFS;
			resetNotifsIndicator();
			break;
		}
		case TAB_MSGS:
		{
			m_selectedTab = TAB_MSGS;
			resetMessagIndicator();
			break;
		}
	}
}

//------------------------------------------------------------------------------//

/// Sets whether notifications are enabled.
/**
 * @param enabled true - enables system notifications
 * @param enabled false - disables system notifications
 */
void Backend::notificationsEnabled(bool enabled)
{
	m_notifsEnabled = enabled;
}

//------------------------------------------------------------------------------//

/// Sets cosumers credetials.
/**
 * 
 * @param consumerKey    - consumer(API) key
 * @param consumerSecret - consumer(API) secret
 */
void Backend::setConsumerCred(const QString & consumerKey, const QString & consumerSecret)
{
	m_twitter->setConsumerCred(consumerKey,	consumerSecret);
}

//------------------------------------------------------------------------------//

/// Resets indicator of new notifications.
void Backend::resetNotifsIndicator()
{
	if(m_model != NULL)
	{
		m_latestNotifsIndicator = m_model->resetNotifsIndicator();
	}
	else
	{
		m_latestNotifsIndicator = " ";
	}

	emit latestNotifsChanged(m_latestNotifsIndicator);
}

//------------------------------------------------------------------------------//

/// Latest notifications changed information.
/**
 * @param indicator - tab indicator text
 * @param text      - text of the system notification
 */
void Backend::latestNotifsChangeCallback(const QString& indicator, const QString& text)
{ 
	if(m_notifsEnabled)
	{
		systemTray->showMessage(tr("Notification"), text, QSystemTrayIcon::Information, 3000);
		qApp->processEvents();
	}

	m_latestNotifsIndicator = indicator;

	if(m_selectedTab != TAB_NOTIFS)
		emit latestNotifsChanged(m_latestNotifsIndicator);
	else
		resetNotifsIndicator();
}

//------------------------------------------------------------------------------//

/// Resets indicator of new direct messages.
void Backend::resetMessagIndicator()
{
	if(m_model != NULL)
	{
		m_latestMsgIndicator = m_model->resetMsgIndicator();
	}
	else
	{
		m_latestMsgIndicator = " ";
	}

	emit latestMessagChanged(m_latestMsgIndicator);
}

//------------------------------------------------------------------------------//


/// Latest direct messages changed information.
/**
 * @param indicator - tab indicator text
 * @param text      - text of the system notification
 */
void Backend::latestMessagChangeCallback(const QString& indicator, const QString& text)
{
	if(m_notifsEnabled)
	{
		systemTray->showMessage(tr("Message"), text, QSystemTrayIcon::Information, 3000);
		qApp->processEvents();
	}

	m_latestMsgIndicator = indicator;

	if(m_selectedTab != TAB_MSGS)
		emit latestMessagChanged(m_latestMsgIndicator);
	else
		resetMessagIndicator();
}