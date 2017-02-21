#include "user.h"

/// Creates new User
/**
 * @param name - name of the user.
 * @param userId - id of the user,
 */
User::User(const QString &name, unsigned long long userId) : m_userId(userId)
{
	m_counter_usages = 0;
	m_userName = name;

}

//------------------------------------------------------------------------------//

/// Returns users name.
/**
 * @return name of the user.
 */
QString User::getUserName() const
{
	return m_userName;
}

//------------------------------------------------------------------------------//

///Returns id of the user.
/**
 * @return id of the user.
 */
unsigned long long User::getUserId() const
{
	return m_userId;
}

//------------------------------------------------------------------------------//

int User::getUsages() const
{
	return m_counter_usages;
}

//------------------------------------------------------------------------------//

void User::incrUsages()
{
	m_counter_usages++;
}

//------------------------------------------------------------------------------//

void User::decrUsages()
{
	m_counter_usages--;
}

//------------------------------------------------------------------------------//

///Compares users by id.
bool User::operator==(const User& user) const
{
	if(m_userId == user.getUserId())
		return true;
	else
		return false;
}