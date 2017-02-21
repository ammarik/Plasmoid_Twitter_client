#include "status.h"

///Creates new status(tweet)
/**
 * @param usersContainer - poiter to user container
 * @param status - text of the status
 * @param statusId - id of the status
 * @param userId - authors of the status id
 * @param tagsList - vector of tags in status
 */
Status::Status(UsersContainer* usersContainer, const string &status, unsigned long long statusId, unsigned long long userId, vector<Tag*>* tagsList) : m_userId(userId)
{
	m_retweet = false;
	m_rtUserName = "";
	m_tagsList = tagsList;

	init(usersContainer, status, statusId);
} 

//------------------------------------------------------------------------------//

/// Creates new retweeted status(tweeet).
/**
 * @param usersContainer - poiter to user container
 * @param status - text of the status
 * @param statusId - id of the status
 * @param userId - authors of the status id
 * @param rtName - id of the origin status
 * @param rtId - id of author of the origin status
 * @param tagsList - vector of tags in status
 */
Status::Status(UsersContainer* usersContainer, const string &status, unsigned long long statusId, unsigned long long userId, const string& rtName, unsigned long long rtId, vector<Tag*>* tagsList) : m_userId(userId)
{
	m_retweet = true;
	m_rtUserName = "<a href=\"https://twitter.com/dceweb/status/" +
	 			QString::number(rtId) + 
	 			"\" style=\"text-decoration:none; color:black; font-weight: bold;\">" +
	 			QString::fromStdString(rtName) +
	 			"</a>";
	m_tagsList = tagsList;
	init(usersContainer, status, statusId);
}

//------------------------------------------------------------------------------//

Status::~Status()
{
	for (std::vector<Tag*>::iterator it = m_tagsList->begin() ; it != m_tagsList->end(); ++it)
   	{
     	delete (*it);
   	}	 
   	delete m_tagsList;
}

//------------------------------------------------------------------------------//

///Initialize status
void Status::init(UsersContainer* usersContainer, const string &status, unsigned long long statusId)
{
	
	offset = 0;

	m_statusId = statusId;

	QString qStatus = QString::fromStdString(status);

	for(vector<Tag*>::iterator it = m_tagsList->begin(); it != m_tagsList->end(); ++it)
	{
		offset = (*it)->wrapTag(qStatus, offset);
	}
	m_text = qStatus;


	m_userName = "<a href=\"https://twitter.com/dceweb/status/" +
				QString::number(statusId) + 
				"\" style=\"text-decoration:none; color:black;\">" +
				usersContainer->getUser(m_userId)->getUserName() +
				"</a>";
}


//------------------------------------------------------------------------------//

/// Returns text of the tweet.
/**
 * @return text of the tweet.
 */
QString Status::statusText() const
{
	return m_text;
}

//------------------------------------------------------------------------------//

/// Returns name of author of the status.
/**
 * @return users name
 */
QString Status::statusUserName() const
{
	return m_userName;
}

//------------------------------------------------------------------------------//

/// Returns profile image that bleongs to tweets author.
/**
 * @return path to the profile image
 */
QString Status::statusProfileImage() const
{	
	QString imagePath = Config::instance().getImagesPath();
	imagePath += Config::instance().getImageNamePrefix();
	imagePath += QString::number(m_userId);

	return imagePath;
}

//------------------------------------------------------------------------------//

/// Returns information whether the status was retweeted.
/**
 * @return true - if the tweet was retweeted.
 * @return flase - otherwise.
 */
bool Status::statusRt() const
{
	return m_retweet;
}

//------------------------------------------------------------------------------//

///Returns name of author of the retweeted status.
/**
 * @return name of the author.
 */
QString Status::statusRtUserName() const
{
	return m_rtUserName;
}

//------------------------------------------------------------------------------//

/// Returns id of the status.
/**
 * @return status id
 */
unsigned long long Status::id() const
{
	return m_statusId;//TODO: Nasetovat v konstruktoru, sjednotit typy s m_userId
}