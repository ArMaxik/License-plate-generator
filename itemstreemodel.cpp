#include "itemstreemodel.h"

#include "QRandomGenerator"

ItemsTreeModel::ItemsTreeModel(QGraphicsScene *gs, QObject *parent)
    : QAbstractItemModel (parent)
    , grScene(gs)
{

}

QGraphicsScene *ItemsTreeModel::getGraphicsScene() const
{
    return grScene;
}

void ItemsTreeModel::addItem(BasicItem *item)
{
    emit layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), grScene->items().count(), grScene->items().count());
    grScene->addItem(item);
    endInsertRows();
    emit layoutChanged();
    emit dataChanged(index(0, 0), index(rowCount()-1, 0));
}

QVariant ItemsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    BasicItem *bi = static_cast<BasicItem*>(index.internalPointer());

    QRectF b = bi->boundingRect();

    QString a = bi->getName();

    return QVariant(bi->getName());

}

Qt::ItemFlags ItemsTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);  // А нужно ли
}

QVariant ItemsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section);

    return QVariant();
}

QModelIndex ItemsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    BasicItem *childItem;

    if (!parent.isValid()) {
        childItem = dynamic_cast<BasicItem*>(grScene->items().at(row));
    }
    else {
        QGraphicsItem *parentItem = static_cast<QGraphicsItem*>(parent.internalPointer());
        childItem = dynamic_cast<BasicItem*>(parentItem->childItems().at(row));
    }

    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ItemsTreeModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
//    if (!index.isValid())
//        return QModelIndex();

//    QGraphicsItem *childItem = static_cast<QGraphicsItem*>(index.internalPointer());
//    QGraphicsItem *parentItem = childItem->parentItem();

//    if (parentItem == nullptr)
//        return QModelIndex();
//    int row;
//    if(parentItem->parentItem() == nullptr) {
//        row = grScene->items().indexOf(parentItem);
//    } else {
//        row = parentItem->parentItem()->childItems().indexOf(parentItem);
//    }
//    return createIndex(row, 0, parentItem);
}

int ItemsTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        return grScene->items().count();
    else
        return static_cast<BasicItem*>(parent.internalPointer())->childItems().count();
}

int ItemsTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
