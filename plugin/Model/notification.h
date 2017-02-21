#ifndef NOTIFICATION_H
#define NOTIFICATION_H 

#include <QObject>
#include <string>
#include "usersContainer.h"
#include "listItem.h"

using namespace std;

/// Represtents notification in twitter.
class Notification : public ListItem
{
public:
	Notification(const QString &text, const QString &user);
	
	QString notificationText() const;
	QString notificationUsers() const;

	virtual unsigned long long id() const;

private:
	QString m_text;
	QString m_user;
	unsigned long long m_notificationId;
};

#endif