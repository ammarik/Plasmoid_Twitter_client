#ifndef MODEL_H
#define MODEL_H 

#include "status.h"
#include "statusesModel.h"
#include "notificationsModel.h"
#include "subListedListModel.h"
#include "conversation.h"
#include "conversationsModel.h"
#include "user.h"
#include "tag.h"
#include "hashtag.h"
#include "mention.h"
#include "url.h"
#include "media.h"
#include "usersContainer.h"
#include "../Twitter/Twitter.h"
#include "../JsonCpp/json/json.h"
#include "../config.h"

#include <QObject>
#include <QDebug>
#include <vector>
#include <algorithm>
#include <klocale.h>

typedef enum CHANGE_TYPE {FRIENDS, TWEET, LIKE, RETWEET, DIRECT_MESSAGE, DELETE_STATUS, DELETE_MESSAGE, FOLLOW, QUOTED, UNKNOWN} CHANGE_TYPE;

using namespace std;

/// Main class of model of the twitter client application.
class Model : public QObject
{
	Q_OBJECT

public:
	Model(Twitter* twitter);
	~Model();

	StatusesModel* getStatusesModel();
	NotificatonsModel* getNotificationsModel();
	ListModel* getDirectMessangingModel();
	ConversationModel* getConversationModel(unsigned long long id);
	//User* getUser(unsigned long long userId) const;
	
	void initAll();

	QString resetNotifsIndicator();
	QString resetMsgIndicator();

	void onTwitterChange(const string& rawData);

private slots:
	int modelTwitterChangeCallback(const QString& data);
	int modelTimelineHomeCallback(const QString& data);
	int modelAccountVerifyCredCallback(const QString& data);
	int modelDirectMessagesCallback(const QString& data);
	int modelDirectMessagesSentCallback(const QString& data);
	void modelTwitterReconnect();
	
signals:
	void latestNotifsModelChanged(const QString&, const QString&);
	void latestMsgsModelChanged(const QString&, const QString&);

private:
	void initDirectMessaging();
	void initSignedUserId(const string& rawData);

	void procTimeLineREST(const string& rawData);
	void procDirectMessagingREST(const string& rawData, std::vector<Message*>* v);

	bool detectError(const string& rawData, const Json::Value& root);

	vector <Tag*>* initTags(const Json::Value& root);

	CHANGE_TYPE getTypeOfChange(const Json::Value& root);
	void change_newTweet(const Json::Value& root, bool addFront);
	void change_newLike(const Json::Value& root);
	void change_newRetweet(const Json::Value& root);
	void change_newDirectMessage(const Json::Value& root);
	void change_deleteStatus(const Json::Value& root);
	void change_deleteMessage(const Json::Value& root);
	void change_newFollow(const Json::Value& root);
	void change_newQuoted(const Json::Value& root);

	QString prepareIndicator(int num);

	static bool myCompareFunction(const Message* m1, const Message* m2);

	StatusesModel* m_statusesModel;
	NotificatonsModel* m_notifModel;
	ConversationsModel* m_dirMsgModel;
	Twitter* m_twitter;
	UsersContainer* m_usersContainer;

	int m_numOflatestNotifs;
	int m_numOflatestMsgs;

	string m_dirMsgsRawData;
	string m_dirMsgsSentRawData;

	QString emptyIndicator = " ";
	QString prependIndicator = "[ ";
	QString appendIndicator = " ]";
};

#endif 
