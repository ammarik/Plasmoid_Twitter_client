#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QtQuick/QQuickItem>
#include <QAbstractListModel>
#include <QList>
#include "listItem.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

///Abstract model of list.
class ListModel : public QAbstractListModel
{
	Q_OBJECT
public:
	ListModel(QObject *parent = 0);
	virtual ~ListModel();

	virtual void addBack(ListItem* item);
	virtual void addFront(ListItem* item);
	virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const = 0;
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	void getItems(QList<ListItem*>& items) const;
	
	ListItem* find(unsigned long long itemId) const;
	int findIndex(unsigned long long itemId) const;
	
	void deleteItem(unsigned long long id);
	void clear();

protected:
	virtual QHash<int, QByteArray> roleNames() const = 0;
	QList<ListItem*> m_items;
    
private:
    
};

#endif