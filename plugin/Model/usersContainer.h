#ifndef USERSCONTAINER_H
#define USERSCONTAINER_H 

#include <QObject>
#include <string>
#include <map>
#include "user.h"
#include "../Twitter/Twitter.h"

using namespace std;

///Container that stores and cares about users.
class UsersContainer
{
public:
	UsersContainer(Twitter* twitter);
	~UsersContainer();
	void addUser(const QString & name, unsigned long long userId, const QString & profileImageUrl);
	void delUser(unsigned long long userId);
	User* getUser(unsigned long long userId);
	bool isInContainer(unsigned long long userId) const;

private:
	map<unsigned long long, User*> users;
	Twitter* m_twitter;
};

#endif