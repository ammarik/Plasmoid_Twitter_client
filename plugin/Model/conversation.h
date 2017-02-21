#ifndef CONVERSATION_H_H
#define CONVERSATION_H_H

#include "user.h"
#include "listModel.h"
#include "subListedListItem.h"
#include "conversationModel.h"
#include "../config.h"

/// Represents conversation in direct messaging.
class Conversation : public SubListedListItem
{
    Q_OBJECT
public:
    Conversation(ListItem* message);
    ~Conversation();
    unsigned long long id() const;
    ListModel*  submodel()  const;
    void append(ListItem* message);
    void prepend(ListItem* message);

    QString getCompanionsName() const;
    QString getLastMessage() const;
    QString getLastMessageDate() const;
    QString getCompanionsProfileImage() const;
    User* getSignedUser() const;
    User* getCompanionUser() const;

    virtual bool equals(ListItem* item) const;

private:
    unsigned long long m_id;
    ListModel* m_conversationModel;

    Message* m_lastMessage;
    User* m_userCompanion;
    User* m_userSigned;
};

#endif