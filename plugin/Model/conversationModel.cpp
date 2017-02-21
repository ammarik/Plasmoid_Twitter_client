#include "conversationModel.h"

//------------------------------------------------------------------------------//

ConversationModel::ConversationModel()
{
}

//------------------------------------------------------------------------------//

ConversationModel::~ConversationModel()
{
}

//------------------------------------------------------------------------------//

QVariant ConversationModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    Message* message = dynamic_cast<Message*>(m_items[index.row()]);
    if (role == TextRole)
        return message->getText();
    else if (role == SenderNameRole)
    	return message->getSenderName();
    else if (role == SenderImageRole)
        return message->getSenderProfileImage();
    else if (role == IsCompanionRole)
        return message->isCompanion();
    return QVariant();
}

//------------------------------------------------------------------------------//

QHash<int, QByteArray> ConversationModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[SenderNameRole] = "senderName";
    roles[SenderImageRole] = "senderImage";
    roles[IsCompanionRole] = "isCompanion";
    return roles;
}
