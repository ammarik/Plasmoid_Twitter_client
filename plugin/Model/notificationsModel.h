#ifndef NOTIFICATIONSMODEL_H
#define NOTIFICATIONSMODEL_H

#include <QtQuick/QQuickItem>
#include <QAbstractListModel>
#include <QStringList>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "notification.h"
#include "listModel.h"

using namespace std;

///model of notifications. 
class NotificatonsModel : public ListModel
{
	Q_OBJECT
public:
	enum StatRoles 
	{
       	TextRole = Qt::UserRole + 1,
       	NameRole
    };
	NotificatonsModel();
	virtual ~NotificatonsModel();

	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
protected:
    virtual QHash<int, QByteArray> roleNames() const;
private:
    
};

#endif