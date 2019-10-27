#include "modelitem.h"

// ====================== AbstractModelItem ==========================

AbstractModelItem::AbstractModelItem(AbstractModelItem *parent)
    : QObject()
    , parent(parent)
{

}

AbstractModelItem::~AbstractModelItem()
{
    qDeleteAll(childItems);
}
AbstractModelItem *AbstractModelItem::child(int row)
{
    if (row < 0 || row >= childItems.size())
        return nullptr;
    return childItems.at(row);
}

int AbstractModelItem::childCount() const
{
    return childItems.count();
}

int AbstractModelItem::columnCount() const
{
    return 1;
}

AbstractModelItem *AbstractModelItem::parentItem()
{
    return parent;
}

void AbstractModelItem::clear()
{
    foreach (auto &child, childItems) {
        delete child;
    }
    childItems.clear();
}

int AbstractModelItem::row() const
{
    if (parent)
        return parent->childItems.indexOf(const_cast<AbstractModelItem*>(this));

    return 0;
}

void AbstractModelItem::appendChild(AbstractModelItem *item)
{
    childItems.append(item);
}

// ====================== TreeItem ==========================

TreeItem::TreeItem(BasicItem *item, AbstractModelItem *parent)
    : AbstractModelItem(parent)
    , itemData(item)
{
    if(itemData != nullptr) {
        connect(itemData, &BasicItem::layoutChanged,
                this, [this](){ emit layoutChanged(); });
    }
}

void TreeItem::setSelected(bool selected)
{
    itemData->setSelected(selected);
}

QLayout *TreeItem::getSettingsLayout()
{
    return itemData->getSettingsLayout();
}

bool TreeItem::isSelected()
{
    return itemData->isSelected();
}


QVariant TreeItem::data(int column) const
{
    if (column != 0)
        return QVariant();
    return itemData->getName();
}
BasicItem *TreeItem::getItem() const
{
    return itemData;
}

// ====================== TreeManagerItem ==========================

TreeManagerItem::TreeManagerItem(AbstractManager *item, AbstractModelItem *parent)
    : AbstractModelItem (parent)
    , itemData(item)
{

}

QVariant TreeManagerItem::data(int column) const
{
    if (column != 0)
        return QVariant();
    return itemData->getName();
}

void TreeManagerItem::setSelected(bool selected)
{
    // :(
}

QLayout *TreeManagerItem::getSettingsLayout()
{
    return itemData->getSettingsLayout();
}

bool TreeManagerItem::isSelected()
{
    return false;
}
