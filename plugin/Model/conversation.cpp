#include "conversation.h"
#include "subListedListModel.h"

//------------------------------------------------------------------------------//

/// Creates new converstion
/**
 * @param First Message that determines the conversation.
 */
Conversation::Conversation(ListItem* message)
{
    m_conversationModel = new ConversationModel();

    m_lastMessage = (Message*)message;
    m_conversationModel->addBack(message);

    m_id = m_lastMessage->getConversationId();
    m_userCompanion = m_lastMessage->getUserCompanion();
    m_userSigned = m_lastMessage->getUserSigned();
}

//------------------------------------------------------------------------------//

Conversation::~Conversation()
{
    m_lastMessage = NULL;
    delete m_conversationModel;
}

//------------------------------------------------------------------------------//

/// Returns id of the conversation.
/**
 * @return id of the conversation
 */
unsigned long long Conversation::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------//

/// Returns submodel i.e, model that represents a list of messages in a conversation.
/**
 * @return submodel
 */
ListModel* Conversation::submodel() const
{
    return m_conversationModel;
}

//------------------------------------------------------------------------------//

/// Append new message to the end of the conversation.
/**
 * @param message that is appended to the conversation.
 */
void Conversation::append(ListItem* message)
{
    if(m_conversationModel->find(message->id()) == NULL)
    {
        m_lastMessage = (Message*)message;
        m_conversationModel->addBack(message);
    }
    else
    {
        delete message;
    }
}

//------------------------------------------------------------------------------//

/// Prepend new message to the beginning of the conversation.
/**
 * @param message that is prepended to the conversation.
 */
void Conversation::prepend(ListItem* message)
{
    if(m_conversationModel->find(message->id()) == NULL)
    {
        m_lastMessage = (Message*)message;
        m_conversationModel->addFront(message);
    }
    else
    {
        delete message;
    }
}

//------------------------------------------------------------------------------//

/// Returns companions name.
/**
 * @return name of the companion
 */
QString Conversation::getCompanionsName() const
{
    if(m_userCompanion != NULL)
        return m_userCompanion->getUserName();
    return "Fail";
}

//------------------------------------------------------------------------------//

/// Returns text of the last message in conversation.
/**
 * @return text of the latest message.
 */
QString Conversation::getLastMessage() const
{
    return m_lastMessage->getText();
}

//------------------------------------------------------------------------------//

/// Returns date of the last message in conversation.
/**
 * @return date of the latest message.
 */
QString Conversation::getLastMessageDate() const
{
    return m_lastMessage->getDate().toString();
}

//------------------------------------------------------------------------------//

/// Returns companions profile image.
/**
 * @return profile image of the companion
 */
QString Conversation::getCompanionsProfileImage() const
{
    QString imagePath = Config::instance().getImagesPath();
    imagePath += Config::instance().getImageNamePrefix();
    imagePath += QString::number(m_userCompanion->getUserId());

    return imagePath;
}

//------------------------------------------------------------------------------//

/// Compares message and conversation by id.
/**
 * @param  message
 * @return true - if message and converation have the same id.
 * @return false - otherwise
 */
bool Conversation::equals(ListItem* item) const
{
    Message* m = (Message*)item;
    if(m_id == m->getConversationId())
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------//

/// Returns signed user.
/**
 * @return signed user
 */
User* Conversation::getSignedUser() const
{
    return m_userSigned;
}

//------------------------------------------------------------------------------//

/// Returns companion user.
/**
 * @return companion user.
 */
User* Conversation::getCompanionUser() const
{
    return m_userCompanion;
}
