#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QVariant>
#include <QHash>

///Abstract list item.
class ListItem : public QObject
{
    Q_OBJECT
public :
    ListItem(QObject *parent = 0) : QObject(parent) {}
    virtual ~ListItem() {}
    virtual unsigned long long id() const = 0;
    virtual bool equals(ListItem* item) const { return item == this; };
};

#endif // LISTITEM_H