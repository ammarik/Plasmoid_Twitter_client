#include "conversationsModel.h"

//------------------------------------------------------------------------------//

ConversationsModel::ConversationsModel()
{
}

//------------------------------------------------------------------------------//

void ConversationsModel::addBack(ListItem* message)
{
	int pos = findIndex(message);

    if(pos != -1)
    {
        ((Conversation*)m_items.at(pos))->append(message); 
        moveConversationToTop(pos);
    }
    else
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_items << new Conversation(message);
        endInsertRows();
    }
}

//------------------------------------------------------------------------------//

void ConversationsModel::addFront(ListItem* message)
{
	int pos = findIndex(message);

    if(pos != -1)
    {
        ((Conversation*)m_items.at(pos))->prepend(message);
    }
    else
    {
        beginInsertRows(QModelIndex(), 0, 0);
    	m_items.insert(0, new Conversation(message));
    	endInsertRows();
    }
}

//------------------------------------------------------------------------------//

void ConversationsModel::moveConversationToTop(int pos)
{
    ListItem* tmpConv = m_items.at(pos);

    beginRemoveRows(QModelIndex(), pos, pos);
    m_items.removeAt(pos);
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, 0);
    m_items.insert(0, tmpConv);
    endInsertRows();
}

//------------------------------------------------------------------------------//

QVariant ConversationsModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    Conversation* conversation = (Conversation*)m_items[index.row()];
    if (role == NameRole)
        return conversation->getCompanionsName();
    else if (role == LastMessageRole)
    	return conversation->getLastMessage();
    else if (role == ImageRole)
        return conversation->getCompanionsProfileImage();
    else if (role == ConversationIdRole)
        return conversation->id();
    return QVariant();
}

//------------------------------------------------------------------------------//

QHash<int, QByteArray> ConversationsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[LastMessageRole] = "lastMessage";
    roles[ImageRole] = "image";
    roles[ConversationIdRole] = "conversationId";
    return roles;
}

//------------------------------------------------------------------------------//

int ConversationsModel::findIndex(ListItem* item) const
{
    for (int i = 0; i < m_items.size(); ++i) 
    {
        if (((Conversation*)m_items.at(i))->equals(item))
            return i;
    }
    return -1;
}