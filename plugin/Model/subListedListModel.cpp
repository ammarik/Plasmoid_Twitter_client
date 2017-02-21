#include "subListedListModel.h"

/*!
 * Construcst a new Models::SubListedListModel instance with the provided \a prototype item template.
 */

SubListedListModel::SubListedListModel(QObject *parent) : ListModel(parent)
{
}

//------------------------------------------------------------------------------//

/*!
 * Returns the model contained by row Item identified by \a id.
 */
ListModel* SubListedListModel::subModelFromId(unsigned long long itemId) const
{
    SubListedListItem* item = (SubListedListItem *)find(itemId);
    if (item != NULL)
    {
        return item->submodel();
    }
    return NULL;
}

//------------------------------------------------------------------------------//

///Deletes sub item.
/**
 * @param subListedItemId - id of item that contains list from should be item deleted.
 * @param itemId - id of the item to delete.
 */
void SubListedListModel::deleteSubItem(unsigned long long subListedItemId, unsigned long long itemId)
{
	ListModel* conversationModel = subModelFromId(subListedItemId);
	if(conversationModel != NULL)
		conversationModel->deleteItem(itemId);
	else
		qCritical() << "Nenalezeno " << subListedItemId;
}