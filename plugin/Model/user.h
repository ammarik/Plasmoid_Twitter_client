#ifndef USER_H
#define USER_H 

#include <QObject>
#include <QUrl>
#include <string>

using namespace std;

/// Represents user.
class User
{
public:
	User(const QString &name, unsigned long long userId);
	
	QString getUserName() const;
	unsigned long long getUserId() const;

	int getUsages() const;
	void incrUsages();
	void decrUsages();

	bool operator==(const User& user) const;	
private:
	int m_counter_usages;

	QString m_userName;
	QUrl m_profile_image;
	unsigned long long m_userId;
};

#endif