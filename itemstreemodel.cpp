#include "itemstreemodel.h"

// ============== TreeItem ==============
TreeItem::TreeItem(BasicItem *item, TreeItem *parent)
    : m_itemData(item), m_parentItem(parent)
{}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}
TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return 1;
}

QVariant TreeItem::data(int column) const
{
    if (column != 0)
        return QVariant();
    return m_itemData->getName();
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

// ============== ItemsTreeModel ==============

ItemsTreeModel::ItemsTreeModel(QObject *parent)
    : QAbstractItemModel (parent)
    , rootItem(new TreeItem(nullptr, nullptr))
{

}

void ItemsTreeModel::addItem(BasicItem *item)
{
//    emit layoutAboutToBeChanged();
//    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    rootItem->appendChild(new TreeItem(item, rootItem));

//    endInsertRows();
//    emit layoutChanged();
//    emit dataChanged(index(0, 0), index(rowCount()-1, 0));
}

QVariant ItemsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    BasicItem *bi = static_cast<BasicItem*>(index.internalPointer());

    return QVariant(bi->getName());

}

Qt::ItemFlags ItemsTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::NoItemFlags;
}

QVariant ItemsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(section);

    return QVariant();
}

QModelIndex ItemsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
//    if (!hasIndex(row, column, parent))
//        return QModelIndex();

//    BasicItem *childItem;

//    if (!parent.isValid()) {
//        childItem = dynamic_cast<BasicItem*>(grScene->items().at(row));
//    }
//    else {
//        QGraphicsItem *parentItem = static_cast<QGraphicsItem*>(parent.internalPointer());
//        childItem = dynamic_cast<BasicItem*>(parentItem->childItems().at(row));
//    }

//    if (childItem)
//        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ItemsTreeModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
//    if (!index.isValid())
//        return QModelIndex();

//    BasicItem *childItem = static_cast<BasicItem*>(index.internalPointer());
//    BasicItem *parentItem = static_cast<BasicItem*>(childItem->parentItem());

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
//    if (parent.column() > 0)
//        return 0;

//    if (!parent.isValid())
//        return grScene->items().count();
//    else
//        return static_cast<BasicItem*>(parent.internalPointer())->childItems().count();
}

int ItemsTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

Qt::DropActions ItemsTreeModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
