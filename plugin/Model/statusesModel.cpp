#include "statusesModel.h"

//------------------------------------------------------------------------------//

StatusesModel::StatusesModel()
{
}

//------------------------------------------------------------------------------//

StatusesModel::~StatusesModel()
{

}

//------------------------------------------------------------------------------//

QVariant StatusesModel::data(const QModelIndex & index, int role) const 
{
    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    Status* status = dynamic_cast<Status*>(m_items[index.row()]);
    if (role == TextRole)
        return status->statusText();
    else if (role == NameRole)
    	return status->statusUserName();
    else if (role == ImageRole)
        return status->statusProfileImage();
    else if (role == IsRetweetRole)
        return status->statusRt();
    else if (role == RetweetedsNameRole)
        return status->statusRtUserName();
    return QVariant();
}

//------------------------------------------------------------------------------//

QHash<int, QByteArray> StatusesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[NameRole] = "name";
    roles[ImageRole] = "image";
    roles[IsRetweetRole] = "is_retweet";
    roles[RetweetedsNameRole] = "retweeteds_user_name";
    return roles;
}

//------------------------------------------------------------------------------//
