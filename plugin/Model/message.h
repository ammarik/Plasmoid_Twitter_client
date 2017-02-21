#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include "user.h"
#include "listItem.h"
#include "../config.h"
#include "./ModelExceptions/missingSignedUserException.h"


/// Represtents direct message.
class Message : public ListItem
{
    Q_OBJECT
public:
    Message(User* sender, User* recipient, const QString& text, const QString& date, unsigned long long messageId) throw(MissingSignedUserException&);
    virtual ~Message();

    virtual unsigned long long id() const;
    unsigned long long getConversationId() const;
    QString getText() const;
    QString getSenderName() const;
    QString getSenderProfileImage() const;
    QDateTime getDate() const;
    bool isCompanion() const;

    User* getUserCompanion() const;
    User* getUserSigned() const;

    bool operator<(const Message& c) const;
    
private:
    void initUsers(User* userA, User* userB) throw(MissingSignedUserException&);
    void setDateTime(const QString& dateTimeString);
    int getMonthNumber(const QString& month);

    User* m_userSender;
	User* m_userSigned;
    User* m_userCompanion;
    unsigned long long m_messageId;
    unsigned long long m_conversationId;
	QString m_text;
    QString m_date;

    QDateTime m_dateTime;
    

};

#endif