#include "notificationsModel.h"

//------------------------------------------------------------------------------//

NotificatonsModel::NotificatonsModel()
{

}

//------------------------------------------------------------------------------//

NotificatonsModel::~NotificatonsModel()
{

}

//------------------------------------------------------------------------------//

QVariant NotificatonsModel::data(const QModelIndex & index, int role) const 
{
    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    Notification *notification = dynamic_cast<Notification*>(m_items[index.row()]);
    if (role == TextRole)
        return notification->notificationText();
    else if (role == NameRole)
    	return notification->notificationUsers();
    return QVariant();
}

//------------------------------------------------------------------------------//

QHash<int, QByteArray> NotificatonsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[NameRole] = "name";
    return roles;
}

//------------------------------------------------------------------------------//