#ifndef SUBLISTEDLISTMODEL_H
#define SUBLISTEDLISTMODEL_H

#include "listModel.h"
#include "subListedListItem.h"

///Abstract model of sub listed list.
class SubListedListModel : public ListModel
{
    Q_OBJECT

public:
    explicit SubListedListModel(QObject *parent = 0);
    Q_INVOKABLE ListModel* subModelFromId(unsigned long long id) const;

    void deleteSubItem(unsigned long long subListedItemId, unsigned long long itemId);

    
};

#endif // SUBLISTEDLISTMODEL_H