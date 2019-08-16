#include "itemsoverview.h"

ItemsOverview::ItemsOverview(ItemsTreeModel *model, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , treeView(new QTreeView())
{

//    ItemsTreeModel *model = new ItemsTreeModel(gs);
    treeView->setModel(model);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);
    setWidget(treeView);
    setWindowTitle(tr("Items overview"));

    show();

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ItemsOverview::onCurrentChanged);
}

void ItemsOverview::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(previous.isValid())
        static_cast<TreeItem*>(previous.internalPointer())->getItem()->parentItem()->setSelected(false);
    if(current.isValid())
        static_cast<TreeItem*>(current.internalPointer())->getItem()->parentItem()->setSelected(true);
}

void ItemsOverview::onSelectionChanged()
{
    treeView->selectionModel()->setCurrentIndex(findSelected(),
                                                QItemSelectionModel::ClearAndSelect);
}

QModelIndex ItemsOverview::findSelected(QModelIndex start)
{
    if(start.isValid()) {
        if(static_cast<TreeItem*>(start.internalPointer())->getItem()->parentItem()->isSelected()) {
            return start;
        }
    }
    for(int i = 0; i < treeView->model()->rowCount(start); i++) {
        QModelIndex res = findSelected(treeView->model()->index(i, 0, start));
        if(res.isValid()) {
            return res;
        }
    }
    return QModelIndex();
}
