#ifndef STATUSESMODEL_H
#define STATUSESMODEL_H

#include <QtQuick/QQuickItem>
#include <QAbstractListModel>
#include <QStringList>
#include "status.h"
#include "listModel.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

///model of statuses. 
class StatusesModel : public ListModel
{
public:
	enum StatRoles 
	{
       	TextRole = Qt::UserRole + 1,
       	NameRole,
       	ImageRole,
       	IsRetweetRole,
       	RetweetedsNameRole
    };
	StatusesModel();
	virtual ~StatusesModel();

	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
protected:
    virtual QHash<int, QByteArray> roleNames() const;
private:
    
};

#endif