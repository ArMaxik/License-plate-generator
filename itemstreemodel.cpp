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

BasicItem *TreeItem::getItem() const
{
    return m_itemData;
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
    emit layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    rootItem->appendChild(new TreeItem(item, rootItem));

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

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());

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
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ItemsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ItemsTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ItemsTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;  // Hmm
}

Qt::DropActions ItemsTreeModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
