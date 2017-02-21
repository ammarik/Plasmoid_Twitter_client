#ifndef STATUS_H
#define STATUS_H 

#include <QObject>
#include <string>
#include "usersContainer.h"
#include "listItem.h"
#include "tag.h"
#include <QDebug>

using namespace std;

///Represents status(tweeet).
class Status : public ListItem
{
	Q_OBJECT
public:
	Status(
		UsersContainer* usersContainer,
		const string &status, 
		unsigned long long statusId, 
		unsigned long long userId, 
		vector<Tag*>* tagsList);
	Status(
		UsersContainer* usersContainer, 
		const string &status, 
		unsigned long long statusId, 
		unsigned long long userId, 
		const string& rtName, 
		unsigned long long rtId, 
		vector<Tag*>* tagsList);
	~Status();
	
	QString statusText() const;
	QString statusUserName() const;
	QString statusProfileImage() const;
	bool statusRt() const;
	QString statusRtUserName() const;

	virtual unsigned long long id() const;
	
private:
	void init(
		UsersContainer* usersContainer, 
		const string &status, 
		unsigned long long statusId);

	QString m_text;
	QString m_userName;
	unsigned long long m_statusId;
	QString m_rtUserName;
	unsigned long long m_userId;
	bool m_retweet;
	vector<Tag*>* m_tagsList;
	

	int offset;
};

#endif