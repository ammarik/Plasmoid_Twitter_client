#include "notification.h"

/// Creates new notification.
/**
 * @param text - text of the notification.
 * @paraam user - originator user of the notification.
 */
Notification::Notification(const QString &text, const QString &user)
{
	m_text = text;
	m_user = user;
	m_notificationId = 0;
} 

//------------------------------------------------------------------------------//

/// Returns text of the notification.
/**
 * @return text of the notification
 */
QString Notification::notificationText() const
{
	return m_text;
}

//------------------------------------------------------------------------------//

/// Returns originator of the notification.
/**
 * @return user - originator of the notification.
 */
QString Notification::notificationUsers() const
{
	return m_user;
}

//------------------------------------------------------------------------------//

/// Returns notifications id.
/**
 * @return id of the notification.
 */
unsigned long long Notification::id() const 
{
  return m_notificationId; //TODO: set in constructor
}
