#include "listModel.h"

//------------------------------------------------------------------------------//

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
}

//------------------------------------------------------------------------------//

ListModel::~ListModel()
{
    clear();
}

//------------------------------------------------------------------------------//

/// Adds list item at the end of the list.
/**
 * @param item - list item to add.
 */
void ListModel::addBack(ListItem* item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items << item;
    endInsertRows();
}

//------------------------------------------------------------------------------//

/// Adds list item at the begining of the list.
/**
 * @param item - list item to add.
 */
void ListModel::addFront(ListItem* item)
{ 
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.insert(0, item);
    endInsertRows();
}

//------------------------------------------------------------------------------//

/// Returns nuber of items in list.
/**
 * @param  parent 
 * @return nuber of items in list.       
 */
int ListModel::rowCount(const QModelIndex & parent) const 
{
    Q_UNUSED(parent);
    return m_items.count();
}

//------------------------------------------------------------------------------//

/// Return list of items.
/**
 * @param[out] items - list of items from list model.
 */
void ListModel::getItems(QList<ListItem*>& items) const
{
	items = m_items;
}

//------------------------------------------------------------------------------//

/// Returns list item with given id.
/**
 * @param  itemId - id of list item.
 * @return list item with given id.
 * @return NULL if there is not list item with given id.
 */
ListItem*  ListModel::find(unsigned long long itemId) const
{
    foreach(ListItem *item, m_items)
    {
        if (item->id() == itemId)

            return item;
    }
    return NULL;
}

//------------------------------------------------------------------------------//

/// Returns position of list item with given id in the list.
/**
 * @param  itemId - id of the list item.
 * @return  position of the list item in list model.
 */
int ListModel::findIndex(unsigned long long itemId) const
{
    for (int i = 0; i < m_items.size(); ++i) 
    {
        if (m_items.at(i)->id() == itemId)
            return i;
    }
    return -1;
}

//------------------------------------------------------------------------------//

/// Deletes item with given id.
/**
 * @param id of the list item to delete
 */
void ListModel::deleteItem(unsigned long long id)
{
    int position = findIndex(id);

    if(position >= 0)
    {
        beginRemoveRows(QModelIndex(), position, position);
        delete m_items.at(position);
        m_items.removeAt(position);
        endRemoveRows();
    }
    else
    {
        qCritical() << "Not Found - Deleting list item with id: " << id;
    }
}


//------------------------------------------------------------------------------//

/// Clears list.
void ListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount()); 
    while (!m_items.isEmpty())
        delete m_items.takeFirst();
    endRemoveRows();
}