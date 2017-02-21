#include "usersContainer.h"


UsersContainer::UsersContainer(Twitter* twitter) : m_twitter(twitter)
{

}

//------------------------------------------------------------------------------//

UsersContainer::~UsersContainer()
{
	std::map<unsigned long long, User*>::iterator it;
	for(it=users.begin(); it!=users.end(); it++)
	{
		delete it->second;
		//users.erase(it);                       //TODO: delete profile image
	}
}

//------------------------------------------------------------------------------//

///Creates and adds user into container.
/**
 * @param name - name of the user
 * @param userId - id of the user
 * @param profileImageUrl - profile image url of the user
 */
void UsersContainer::addUser(const QString & name, unsigned long long userId, const QString & profileImageUrl)
{
	if(isInContainer(userId))
	{
		users[userId]->incrUsages();
	}
	else
	{
		User* user = new User(name, userId);
		users[userId] = user;
		users[userId]->incrUsages();
		QString imageFilename = QString::number(userId);
		m_twitter->downloadProfileImage(imageFilename, profileImageUrl);
	}
}

//------------------------------------------------------------------------------//

///Deletes user.
/**
 * @param userId - id of user to delete.
 */
void UsersContainer::delUser(unsigned long long userId)
{
    if(isInContainer(userId))
	{
		users[userId]->decrUsages();
		if(users[userId]->getUsages() <= 0)
		{
			delete users[userId];
			users.erase(userId);           //TODO: delete profile image
		}
	}
}

//------------------------------------------------------------------------------//

/// Returns user with given id.
/**
 * @param  userId - id of the returned user.
 * @return User with given id, if exists. Otherwise NULL.
 */
User* UsersContainer::getUser(unsigned long long userId)
{
	return users[userId];
}

//------------------------------------------------------------------------------//

/// Return information if user with given if is in UsersContainer.
/**
 * @param  userId - id of user.
 * @return true - if user with given id is in container.
 * @return false - if user with given id isn't in container. 
 */
bool UsersContainer::isInContainer(unsigned long long userId) const
{
	if(users.find(userId) == users.end())
		return false;
	else
		return true;
}