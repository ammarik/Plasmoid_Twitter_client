#include "model.h"

/// Creates new instance of model.
/**
 * @param twitter - twitter object, that is used for comunnication with Twitter server.
 */
Model::Model(Twitter* twitter) : m_twitter(twitter)
{
	m_statusesModel = new StatusesModel();
	m_notifModel = new NotificatonsModel();
	m_dirMsgModel = new ConversationsModel();
	m_usersContainer = new UsersContainer(m_twitter);

	m_dirMsgsRawData = "";
	m_dirMsgsSentRawData = "";

	m_numOflatestNotifs = 0;
} 

//------------------------------------------------------------------------------//

Model::~Model()
{
	delete m_statusesModel;
	delete m_notifModel;
	delete m_dirMsgModel;
	delete m_usersContainer;
}

//------------------------------------------------------------------------------//

/// Returns model of tweets(statuses).
/**
 * @return StatusesModel
 */
StatusesModel* Model::getStatusesModel()
{
	return m_statusesModel; 
}

//------------------------------------------------------------------------------//

/// Returns model of notifications.
/**
 * @return NotificationsModel
 */
NotificatonsModel* Model::getNotificationsModel()
{
	return m_notifModel;
}

//------------------------------------------------------------------------------//

/// Returns model of direct messaging.
/**
 * @return ConversationsModel
 */
ListModel* Model::getDirectMessangingModel()
{
	return m_dirMsgModel;
}

//------------------------------------------------------------------------------//

/// Returns model of conversation with given id.
/**
 * @param  id - id of the conversation.
 * @return ConversationModel with given id.
 */
ConversationModel* Model::getConversationModel(unsigned long long id)
{
	ListModel* lm = m_dirMsgModel->subModelFromId(id);

	if(lm != NULL)
		return dynamic_cast<ConversationModel*>(m_dirMsgModel->subModelFromId(id));
	return NULL;
}


//------------------------------------------------------------------------------//

void Model::initAll()
{
	m_twitter->startStream();
	connect(m_twitter, SIGNAL(sig_twit_timelineHome(QString)), this, SLOT(modelTimelineHomeCallback(QString)));
	connect(m_twitter, SIGNAL(sig_twit_accountVerifyCred(QString)), this, SLOT(modelAccountVerifyCredCallback(QString)));
	connect(m_twitter, SIGNAL(sig_twit_directMessages(QString)), this, SLOT(modelDirectMessagesCallback(QString)));
	connect(m_twitter, SIGNAL(sig_twit_directMessagesSent(QString)), this, SLOT(modelDirectMessagesSentCallback(QString)));
	connect(m_twitter, SIGNAL(sig_twit_twitterChanged(QString)), this, SLOT(modelTwitterChangeCallback(QString)));
	connect(m_twitter, SIGNAL(sig_twit_reconnect()), this, SLOT(modelTwitterReconnect()));
}

//------------------------------------------------------------------------------//

/// Inits direct messaging
void Model::initDirectMessaging()
{
	std::vector<Message*>* v = new std::vector<Message*>();

	procDirectMessagingREST(m_dirMsgsRawData, v);
	procDirectMessagingREST(m_dirMsgsSentRawData, v);
	
	sort(v->begin(), v->end(), myCompareFunction);
	
	for (std::vector<Message*>::iterator it = v->begin(); it != v->end(); ++it)
	{
		m_dirMsgModel->addBack( *it );
	}
	delete v;
}

//------------------------------------------------------------------------------//

bool Model::myCompareFunction(const Message* m1, const Message* m2)
{
	return (*m1 < *m2);
}

//------------------------------------------------------------------------------//

/// Intins signed-in user.
/**
 * @param rawData -  raw string directly from the server.
 */
void Model::initSignedUserId(const string& rawData)
{
	Json::Value root;
	Json::Reader reader;

	bool parsedSucess = reader.parse(rawData, root, false);
	if(! parsedSucess)
	{
		qCritical() << "Failed to parse JSON";
		qCritical() << QString::fromStdString(reader.getFormattedErrorMessages());
		return;
	}

	unsigned long long userId = root["id"].asUInt64();
	QString name = QString::fromStdString(root["name"].asString());
	QString profileImageUrl = QString::fromStdString(root["profile_image_url"].asString());

	m_usersContainer->addUser(name, userId, profileImageUrl);
	Config::instance().setSignedUserId(userId);
}

//------------------------------------------------------------------------------//

/// Resets model of the notifications indicator.
/**
 * @return value of the indicator.
 */
QString Model::resetNotifsIndicator()
{
	m_numOflatestNotifs = 0;
	return emptyIndicator;
}

//------------------------------------------------------------------------------//

/// Resets model of the messages indicator.
/**
 * @return value of the indicator.
 */
QString Model::resetMsgIndicator()
{
	m_numOflatestMsgs = 0;
	return emptyIndicator;
}

//------------------------------------------------------------------------------//

/// Processes changes on the server.
/**
 * @param rawData - raw string from the server.
 */
void Model::onTwitterChange(const string& rawData)
{
	Json::Value root;
	Json::Reader reader;

	cout << rawData << endl;

	if(rawData.find('{') != string::npos)
	{
		bool parsedSucess = reader.parse(rawData, root, false);
		if(! parsedSucess)
		{
			qCritical() << "Failed to parse JSON";
			qCritical() << QString::fromStdString(reader.getFormattedErrorMessages());
			return;
		}

		if(!detectError(rawData, root))
			return;
	
		CHANGE_TYPE type = getTypeOfChange(root);

	  	switch(type)
	  	{
			case TWEET:
				change_newTweet(root, true);
				break;
			case LIKE:
				change_newLike(root);
				break;
			case RETWEET:
			{
				change_newTweet(root, true);
				change_newRetweet(root);
				break;
			}
			case DIRECT_MESSAGE:
				change_newDirectMessage(root["direct_message"]);
				break;
			case DELETE_STATUS:
				change_deleteStatus(root);
				break;
			case DELETE_MESSAGE:
				change_deleteMessage(root);
				break;
			case FOLLOW:
				change_newFollow(root);
				break;
			case QUOTED:
				change_newQuoted(root);
			case FRIENDS:
				break;
			default:
				qInfo() << "Invalid type of twitter change: " << QString::fromStdString(rawData);
				break;
		}
	}
}

//------------------------------------------------------------------------------//

/// Recognizes type of the change.
/**
 * @param  root - JSON parsed server chnage.
 * @return CHANGE_TYPE - type of the change on the server.   
 */
CHANGE_TYPE Model::getTypeOfChange(const Json::Value& root)
{
	if(root.isObject())
	{
		if(root.isMember("friends"))
		{
			qInfo() << "FRIENDS";
			return FRIENDS;
		}
		else if(root.isMember("event") && (root["event"] == "favorite"))
		{
			qInfo() << "LIKE";
			return LIKE;
		}
		else if (root.isMember("retweeted_status"))
		{
			qInfo() << "RETWEET";
			return RETWEET;
		}
		else if(root.isMember("direct_message"))
		{
			qInfo() << "DIRECT_MESSAGE";
			return DIRECT_MESSAGE;
		}
		else if(root.isMember("favorited"))
		{
			qInfo() << "TWEET";
			return TWEET;
		}
		else if(root.isMember("event") && (root["event"] == "follow"))
		{
			qInfo() << "FOLLOW";
			return FOLLOW;
		}
		else if(root.isMember("event") && (root["event"] == "quoted_tweet"))
		{
			qInfo() << "QUOTED";
			return  QUOTED;
		}
		else if(root.isMember("delete"))
		{
			if(root["delete"].isMember("status"))
			{
				qInfo() << "DELETE_STATUS";
				return DELETE_STATUS;
			}
			else if(root["delete"].isMember("direct_message"))
			{
				qInfo() << "DELETE_MESSAGE";
				return DELETE_MESSAGE;
			}
		}
	}	

	qInfo() << "UNKNOWN";
	return UNKNOWN;
}

//------------------------------------------------------------------------------//

/// Processes new tweet change.
/**
 * @param root - parsed JSON value.
 * @param addFront true - true if new tweet sould be added to the beginning of conversation.
 * @param addFront false - false if new tweet should be added to the end of conversation.
 */
void Model::change_newTweet(const Json::Value& root, bool addFront)
{
	Status* status;
	string text, retweetedsName;  
	
	QString name = QString::fromStdString(root["user"]["name"].asString());
	QString profileImageUrl = QString::fromStdString(root["user"]["profile_image_url"].asString());
	unsigned long long userId = root["user"]["id"].asUInt64();
	unsigned long long statusId = root["id"].asUInt64();

	m_usersContainer->addUser(name, userId, profileImageUrl);	

	if(root.isObject() && root.isMember("retweeted_status"))
	{
		retweetedsName = root["retweeted_status"]["user"]["name"].asString();
		text = root["retweeted_status"]["text"].asString();
		unsigned long long retweetedStatusId = root["retweeted_status"]["id"].asUInt64();
		vector<Tag*>* tagsList = initTags(root["retweeted_status"]["entities"]);
		status = new Status(m_usersContainer, text, statusId, userId, retweetedsName, retweetedStatusId, tagsList);
	}
	else
	{
		text = root["text"].asString();
		vector<Tag*>* tagsList = initTags(root["entities"]);
		status = new Status(m_usersContainer, text, statusId, userId, tagsList);
	}	
	
	if(m_statusesModel->find(status->id()) == NULL)
	{
		if(addFront)
		{
			m_statusesModel->addFront(status);
		}
		else
		{
			m_statusesModel->addBack(status);
		}
	}
}

//------------------------------------------------------------------------------//

/// Inits tags(Hashtag, Mention, Media, Url).
/**
 * @param root - parsed JSON value.
 * @return vector of pointers to tags.
 */
vector <Tag*>* Model::initTags(const Json::Value& root)
{
	const int numOfEntities = 4;
	const string entities[] = {"hashtags", "user_mentions", "urls", "media"};

	vector<Tag*>* tagsList = new vector<Tag*>;
	Tag* tag;


    for(int i = 0; i < numOfEntities; i++)
    {
        for (unsigned int j = 0; j < root[entities[i]].size(); ++j)
		{
			Json::Value entity = root[entities[i]][j];

			switch(i)
			{
    			case 0 : 
    				tag = new Hashtag(
    					entity["indices"][0].asInt(),
    				 	entity["indices"][1].asInt(),
    				 	entity["text"].asString());
    				break;
    			case 1 : 
    				tag = new Mention(
    					entity["indices"][0].asInt(), 
    					entity["indices"][1].asInt(), 
    					entity["id"].asUInt64(), 
    					entity["name"].asString(), 
    					entity["screen_name"].asString());
    				break;
    			case 2 : 
    				tag = new Url(
    					entity["indices"][0].asInt(), 
    					entity["indices"][1].asInt(),
    					entity["display_url"].asString(), 
    					entity["expanded_url"].asString(), 
    					entity["url"].asString());
    				break;
    			case 3 : 
    				tag = new Media(
    					entity["indices"][0].asInt(), entity["indices"][1].asInt(),
	 					entity["display_url"].asString(),
	 					entity["expanded_url"].asString(), 
	 					entity["media_url"].asString(),
	 					entity["media_url_https"].asString(),
	 					entity["id"].asUInt64());
    			   	break;
			}

			vector<Tag*>::iterator itToInsert = std::lower_bound(
				tagsList->begin(),
				tagsList->end(), 
				tag, 
				[](const Tag* lhs, const Tag* rhs) { return *lhs < *rhs; });
			tagsList->insert(itToInsert, tag);
		}	
    }

    return tagsList;
}

//------------------------------------------------------------------------------//

/// Processes new like change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_newLike(const Json::Value& root)
{
	QString name = QString::fromStdString(root["source"]["name"].asString());
	QString text = QString::fromStdString(root["target_object"]["text"].asString());

	QString liked = i18n(" liked ");
	liked.append(text);

	Notification* notifi = new Notification(liked, name);
	m_notifModel->addFront(notifi);

	emit latestNotifsModelChanged(prepareIndicator(++m_numOflatestNotifs), name.append(liked));
}

//------------------------------------------------------------------------------//

/// Processes new follow change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_newFollow(const Json::Value& root)
{
	QString name = QString::fromStdString(root["source"]["name"].asString());
	QString text = QString::fromStdString(root["target_object"]["text"].asString());
	unsigned long long id  = root["source"]["id"].asUInt64();

	QString foll = i18n(" started to follow you.");

	if(id !=  Config::instance().getSignedUserId())
	{
		Notification* notifi = new Notification(foll, name);
		m_notifModel->addFront(notifi);

		emit latestNotifsModelChanged(prepareIndicator(++m_numOflatestNotifs), name.append(foll));
	}
}

//------------------------------------------------------------------------------//

/// Processes new quote change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_newQuoted(const Json::Value& root)
{
	QString name = QString::fromStdString(root["source"]["name"].asString());
	QString text = QString::fromStdString(root["target_object"]["quoted_status"]["text"].asString());
	unsigned long long id  = root["source"]["id"].asUInt64();

	QString rt = i18n(" retweeted your tweet ");
	rt.append("\"").append(text).append("\"");

	if(id !=  Config::instance().getSignedUserId())
	{
		Notification* notifi = new Notification(rt, name);
		m_notifModel->addFront(notifi);
	
		emit latestNotifsModelChanged(prepareIndicator(++m_numOflatestNotifs), name.append(rt));
	}
}

//------------------------------------------------------------------------------//

/// Processes new retweet change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_newRetweet(const Json::Value& root)
{
	unsigned long long id = root["retweeted_status"]["user"]["id"].asUInt64();

	if(id == Config::instance().getSignedUserId())
	{
		QString text = QString::fromStdString(root["text"].asString());
		QString name = QString::fromStdString(root["user"]["name"].asString());

		QString retweetedBy = i18n(" retweeted ");
		retweetedBy.append(text);

		Notification* notifi = new Notification(retweetedBy, name);
		m_notifModel->addFront(notifi);

		emit latestNotifsModelChanged(prepareIndicator(++m_numOflatestNotifs), name.append(retweetedBy));
	}
}

//------------------------------------------------------------------------------//

/// Processes new direct message change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_newDirectMessage(const Json::Value& root)
{
		unsigned long long messageId = root["id"].asUInt64();
		QString text = QString::fromStdString(root["text"].asString());
		QString date = QString::fromStdString(root["created_at"].asString());
		unsigned long long senderId = root["sender"]["id"].asUInt64();

		QString senderName = QString::fromStdString(root["sender"]["name"].asString());
		QString recipientName = QString::fromStdString(root["recipient"]["name"].asString());

		QString senderProfileImageUrl = QString::fromStdString(root["sender"]["profile_image_url"].asString());
		QString recipientProfileImageUrl = QString::fromStdString(root["recipient"]["profile_image_url"].asString());

		unsigned long long senderUserId = root["sender"]["id"].asUInt64();
		unsigned long long recipientUserId = root["recipient"]["id"].asUInt64();

		m_usersContainer->addUser(senderName, senderUserId, senderProfileImageUrl);
		m_usersContainer->addUser(recipientName, recipientUserId, recipientProfileImageUrl);

		try
		{
		Message* message = new Message(
			m_usersContainer->getUser(senderUserId),
			m_usersContainer->getUser(recipientUserId),
			text,
			date,
			messageId);
		
		m_dirMsgModel->addBack(message);

		if(senderId != Config::instance().getSignedUserId())
		{
			emit latestMsgsModelChanged(prepareIndicator(++m_numOflatestMsgs), senderName.append(": ").append(text));
		}
		}
		catch(MissingSignedUserException & ex)
		{
			qCritical("%s", ex.what());
		}
}

//------------------------------------------------------------------------------//

/// Processes new delete status change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_deleteStatus(const Json::Value& root)
{
	unsigned long long statusId = root["delete"]["status"]["id"].asUInt64();
	m_statusesModel->deleteItem(statusId);
}

//------------------------------------------------------------------------------//

/// Processes new delete message change.
/**
 * @param root - parsed JSON value.
 */
void Model::change_deleteMessage(const Json::Value& root)
{
	unsigned long long messageId = root["delete"]["direct_message"]["id"].asUInt64();
	unsigned long long companionUserId = root["delete"]["direct_message"]["user_id"].asUInt64();
	m_dirMsgModel->deleteSubItem(companionUserId, messageId);
}

//------------------------------------------------------------------------------//

/// Processes time line data from REST API.
/**
 * @param rawData - raw string from the server.
 */
void Model::procTimeLineREST(const string& rawData)
{
	Json::Value root;
	Json::Reader reader;

	bool parsedSucess = reader.parse(rawData, root, false);
	if(! parsedSucess)
	{
		qCritical() << "Failed to parse JSON";
		qCritical() << QString::fromStdString(reader.getFormattedErrorMessages());
		return;
	}

	if(!detectError(rawData, root))
		return;

	for (unsigned int i = 0; i < root.size(); ++i)
	{
		change_newTweet(root[i], false);
	}
}

//------------------------------------------------------------------------------//

/// Processes direct messaging data from REST API.
/**
 * @param rawData - raw string from the server.
 */
void Model::procDirectMessagingREST(const string& rawData, std::vector<Message*>* v)
{
	Json::Value root;
	Json::Reader reader;

	bool parsedSucess = reader.parse(rawData, root, false);
	if(! parsedSucess)
	{
		qCritical() << "Failed to parse JSON";
		qCritical() << QString::fromStdString(reader.getFormattedErrorMessages());
		return;
	}

	if(!detectError(rawData, root))
		return;

	for(unsigned int i = 0; i < root.size(); ++i)
	{
		unsigned long long messageId = root[i]["id"].asUInt64();
		QString text = QString::fromStdString(root[i]["text"].asString());
		QString date = QString::fromStdString(root[i]["created_at"].asString());

		QString senderName = QString::fromStdString(root[i]["sender"]["name"].asString());
		QString recipientName = QString::fromStdString(root[i]["recipient"]["name"].asString());

		QString senderProfileImageUrl = QString::fromStdString(root[i]["sender"]["profile_image_url"].asString());
		QString recipientProfileImageUrl = QString::fromStdString(root[i]["recipient"]["profile_image_url"].asString());

		unsigned long long senderUserId = root[i]["sender"]["id"].asUInt64();
		unsigned long long recipientUserId = root[i]["recipient"]["id"].asUInt64();

		m_usersContainer->addUser(senderName, senderUserId, senderProfileImageUrl);
		m_usersContainer->addUser(recipientName, recipientUserId, recipientProfileImageUrl);

		try
		{
		v->push_back(new Message(
		 	m_usersContainer->getUser(senderUserId),
		 	m_usersContainer->getUser(recipientUserId),
		 	text,
		 	date,
		 	messageId));
		}
		catch(MissingSignedUserException & ex)
		{
			qCritical("%s", ex.what());
		}
	}

}

//------------------------------------------------------------------------------//

/// Detects error message.
bool Model::detectError(const string& rawData, const Json::Value& root)
{
	if(root.isObject() && root.isMember("errors"))
	{
		qCritical("%s", rawData.c_str());
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------//

int Model::modelTimelineHomeCallback(const QString& data)
{
	string s = data.toUtf8().constData();
    procTimeLineREST(s);
    return 1;
}

//------------------------------------------------------------------------------//

int Model::modelAccountVerifyCredCallback(const QString& data)
{
	string s = data.toUtf8().constData();
    initSignedUserId(s);
    return 1;
}

//------------------------------------------------------------------------------//

int Model::modelDirectMessagesCallback(const QString& data)
{
	string s = data.toUtf8().constData();
    m_dirMsgsRawData = s;
    return 1;
}

//------------------------------------------------------------------------------//

int Model::modelDirectMessagesSentCallback(const QString& data)
{
	string s = data.toUtf8().constData();
    m_dirMsgsSentRawData = s;

    if((m_dirMsgsRawData != "") && (m_dirMsgsSentRawData != ""))
    {
    	initDirectMessaging();
    }
    return 1;
}

//------------------------------------------------------------------------------//

int Model::modelTwitterChangeCallback(const QString& data)
{
	string s = data.toUtf8().constData();
    onTwitterChange(s);
    return 1;
}

//------------------------------------------------------------------------------//

void Model::modelTwitterReconnect()
{
	m_statusesModel->clear();
	m_dirMsgModel->clear();
}

//------------------------------------------------------------------------------//

/// Prepares indicator of change.
/**
 * @param  num - num of changes
 * @return indicator in string form.
 */
QString Model::prepareIndicator(int num)
{
	QString s = QString::number(num);
	s.prepend(prependIndicator);
	s.append(appendIndicator);

	return s;
}
