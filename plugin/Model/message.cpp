#include "message.h"

//------------------------------------------------------------------------------//

/// Creates message instace.
/**
 * @param sender - sender of the message.
 * @param recipient - recipient of the message.
 * @param text - text of the message.
 * @param date - date of the message.
 * @param messageId - id of the message.
 */
Message::Message(User* sender, User* recipient, const QString& text, const QString& date, unsigned long long messageId) throw(MissingSignedUserException&)
{
	m_text = text;
	m_date = date;
	m_messageId = messageId;
	m_userSender = sender;

	initUsers(sender, recipient);

    m_conversationId =  m_userCompanion->getUserId();

    setDateTime(m_date);
}

//------------------------------------------------------------------------------//

Message::~Message()
{
}

//------------------------------------------------------------------------------//

/// Inits signed user and companion user.
/**
 * @param userA - User in conversation.
 * @param userB - User in conversation.
 */
void Message::initUsers(User* userA, User* userB) throw(MissingSignedUserException&)
{
    m_userSigned = NULL;
    m_userCompanion = NULL;
    unsigned long long signedUserId = Config::instance().getSignedUserId();

    if(userA->getUserId() == signedUserId)
    {
        m_userSigned = userA;
        m_userCompanion = userB;
    }
    else if(userB->getUserId() == signedUserId)
    {
        m_userSigned = userB;
        m_userCompanion = userA;
    }
    else
    {
        throw MissingSignedUserException(signedUserId, userA->getUserId(),userB->getUserId());
    }
}

//------------------------------------------------------------------------------//

/// Sets datetime of the message.
/**
 * @param dateTimeString in string form.
 */
void Message::setDateTime(const QString& dateTimeString)
{
    QDateTime dateTime;
    QStringList list1 = dateTimeString.split(' ');

    bool yearOk = false;
    bool dayOk = false;
    bool hourOk = false;
    bool minOk = false;
    bool secOk = false;
    bool offsetOk = false;

    int year, month, day, hour, min, sec, offsetSeconds; 

    if(list1.size() >= 6)
    {
        year = list1.at(5).toInt(&yearOk, 10);
        //qDebug() << "YEAR" << year;

        month = getMonthNumber(list1.at(1));
        //qDebug() << "MONTH" << month;
    
        day = list1.at(2).toInt(&dayOk, 10); 
        //qDebug() << "DAY" << day;

        QStringList list2 = list1.at(3).split(':');
        if(list2.size() >= 3)
        {
            hour = list2.at(0).toInt(&hourOk, 10);
            min = list2.at(1).toInt(&minOk, 10);
            sec = list2.at(2).toInt(&secOk, 10);
        }
        else
        {

        }

        QString offsetStr = list1.at(4);
        offsetSeconds = offsetStr.mid(1,2).toInt(&offsetOk, 10) * 3600;
        offsetSeconds += offsetStr.mid(3,2).toInt(&offsetOk, 10) * 60;
        if(offsetStr.startsWith("-"))
            offsetSeconds = offsetSeconds * (-1);
    }
    else
    {

    }

    if(dayOk && yearOk && (month > 0) && hourOk && minOk && secOk)
    {
        QDate date = QDate(year, month, day);
        dateTime.setDate(date);

        QTime time = QTime(hour, min, sec);
        dateTime.setTime(time);

        QTimeZone timeZone = QTimeZone(offsetSeconds);
        dateTime.setTimeZone(timeZone);

        m_dateTime = dateTime;
        //qDebug() << "DateTime: " << dateTime.toString();
    }
    else
    {

    }
}

//------------------------------------------------------------------------------//

int Message::getMonthNumber(const QString& month)
{
    if(month == "Jan")
    {
        return 1;
    }
    else if(month == "Feb")
    {
        return 2;
    }
    else if(month == "Mar")
    {
        return 3;        
    }
    else if(month == "Apr")
    {
        return 4;        
    }
    else if(month == "May")
    {
        return 5;       
    }
    else if(month == "Jun")
    {
        return 6;
    }
    else if(month == "Jul")
    {
        return 7;
    }
    else if(month == "Aug")
    {
        return 8;
    }
    else if(month == "Sep")
    {
        return 9;
    }
    else if(month == "Oct")
    {
        return 10;
    }
    else if(month == "Nov")
    {
        return 11;
    }
    else if(month == "Dec")
    {
        return 12;
    }
    else
        return -1;
}

//------------------------------------------------------------------------------//

/// Returns id of the message.
/**
 * @return id of the message.
 */
unsigned long long Message::id() const 
{
    return m_messageId;
}

//------------------------------------------------------------------------------//

/// Returns id of conversation in which the message belongs.
/**
 * @return id of conversation.
 */
unsigned long long Message::getConversationId() const
{
	return m_conversationId;
}

//------------------------------------------------------------------------------//

/// Returns text of the message.
/**
 * @return text of the message.
 */
QString Message::getText() const
{
  return m_text;
}

//------------------------------------------------------------------------------//

/// Returns name of sender of the message.
/**
 * @return name of the sender.
 */
QString Message::getSenderName() const
{
  return m_userSender->getUserName();
}

//------------------------------------------------------------------------------//

/// Returns profile image of the message sender.
/**
 * @return path to the profile image of the sender.
 */
QString Message::getSenderProfileImage() const
{
    QString imagePath = Config::instance().getImagesPath();
    imagePath += Config::instance().getImageNamePrefix();
    imagePath += QString::number(m_userSender->getUserId());

    return imagePath;
}

//------------------------------------------------------------------------------//

/// Returns date of the message.
/**
 * @return date of the message.
 */
QDateTime Message::getDate() const
{
	return m_dateTime;
}

//------------------------------------------------------------------------------//

/// Returns information if the message was sent by companion user or not.
/**
 * @return true - If the message was sent by companion.
 * @return false - If the message wasn't sent by companion.
 */
bool Message::isCompanion() const
{
	return (m_userSender == m_userCompanion);
}

//------------------------------------------------------------------------------//

/// Returns comanion user.
/**
 * @return companion user.
 */
User* Message::getUserCompanion() const
{
	return m_userCompanion;
}

//------------------------------------------------------------------------------//

/// Returns signed-in user.
/**
 * @return signed-in user.
 */
User* Message::getUserSigned() const
{
	return m_userSigned;
}

//------------------------------------------------------------------------------//

/// Compares messages by date.
bool Message::operator<(const Message& c) const
{
    return (m_dateTime < c.getDate());
}
    
