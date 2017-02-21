#ifndef CONVERSATIONSMODEL_H
#define CONVERSATIONSMODEL_H

#include <QtQuick/QQuickItem>
#include <QAbstractListModel>
#include <QStringList>
#include <iterator>

#include "status.h"
#include "conversation.h"
#include "subListedListModel.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

/// model of list of conversations.
class ConversationsModel : public SubListedListModel
{
public:
	enum ConvRoles 
	{
       	NameRole = Qt::UserRole + 1,
       	LastMessageRole,
       	ImageRole,
       	ConversationIdRole
    };
	ConversationsModel();

	virtual void addBack(ListItem* item);
	virtual void addFront(ListItem* item);
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	int findIndex(ListItem* item) const;
	void moveConversationToTop(int pos);

protected:
    virtual QHash<int, QByteArray> roleNames() const;
private:
    
};

#endif