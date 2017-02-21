#ifndef SUBLISTEDLISTITEM_H
#define SUBLISTEDLISTITEM_H

#include "listItem.h"
#include "listModel.h"

///Abstract list item with sub list item.
class SubListedListItem : public ListItem
{
    Q_OBJECT
public :
    SubListedListItem(QObject *parent = 0) : ListItem(parent) {}
    virtual ~SubListedListItem() {}

    virtual ListModel*  submodel()  const = 0;
    virtual void append(ListItem* item) = 0;
    virtual void prepend(ListItem* item) = 0;
};

#endif // SUBLISTEDLISTITEM_H