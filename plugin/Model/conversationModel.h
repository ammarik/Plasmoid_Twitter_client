#ifndef CONVERSATIONMODEL_H
#define CONVERSATIONMODEL_H

#include <QtQuick/QQuickItem>
#include <QAbstractListModel>
#include <QStringList>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "status.h"
#include "message.h"
#include "listModel.h"

using namespace std;

/// model of conversation.
class ConversationModel : public ListModel
{
public:
	enum ConvRoles 
	{
       	TextRole = Qt::UserRole + 1,
       	SenderNameRole,
       	SenderImageRole,
       	IsCompanionRole
    };
	ConversationModel();
  virtual ~ConversationModel();
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
protected:
  virtual QHash<int, QByteArray> roleNames() const;
private:
    
};

#endif