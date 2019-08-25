#include "treemodel.h"

AbstractTreeModel::AbstractTreeModel(QObject *parent)
    : QAbstractItemModel (parent)
{

}

QVariant AbstractTreeModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << "parentItem->childCount()";
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());

    return item->data(index.column());

}

Qt::ItemFlags AbstractTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::NoItemFlags;
}

QVariant AbstractTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(tr("Item name"));

    return QVariant();
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    AbstractModelItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AbstractModelItem*>(parent.internalPointer());

    AbstractModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    AbstractModelItem *childItem = static_cast<AbstractModelItem*>(index.internalPointer());
    AbstractModelItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

bool AbstractTreeModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

int AbstractTreeModel::rowCount(const QModelIndex &parent) const
{
    AbstractModelItem *parentItem;
//    if (parent.column() > 0)
//        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AbstractModelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int AbstractTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;  // Hmm
}

Qt::DropActions AbstractTreeModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

// ============== ItemsTreeModel ==============

ItemsTreeModel::ItemsTreeModel(QObject *parent)
    : AbstractTreeModel (parent)
    , canvas(nullptr)
{
    rootItem = new TreeItem(nullptr, nullptr);
}

void ItemsTreeModel::addItem(BasicItem *item)
{
//    emit layoutAboutToBeChanged();
    int pos = rowCount(index(0, 0, QModelIndex()));
    beginInsertRows(index(0, 0, QModelIndex()), pos, pos);

    canvas->appendChild(new TreeItem(item, canvas));

    item->getDiffuseChanel()->setParentItem(canvas->getItem()->getDiffuseChanel());
    item->getSpecularChanel()->setParentItem(canvas->getItem()->getSpecularChanel());

    endInsertRows();
//    emit layoutChanged();
}

void ItemsTreeModel::setCanvas(Canvas *c)
{
    emit layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    canvas = new TreeItem(c, rootItem);
    rootItem->appendChild(canvas);

    endInsertRows();
    emit layoutChanged();
}

// ============== SceneTreeModel ==============

SceneTreeModel::SceneTreeModel(QObject *parent)
    : AbstractTreeModel(parent)
{
    rootItem = new TreeManagerItem(nullptr, nullptr);
}

void SceneTreeModel::addItem(AbstractManager *item)
{
    emit layoutAboutToBeChanged();
    beginInsertRows(index(0, 0),
                    rowCount(index(0, 0)),
                    rowCount(index(0, 0)));

    rootItem->appendChild(new TreeManagerItem(item, rootItem));

    endInsertRows();
    emit layoutChanged();
}
