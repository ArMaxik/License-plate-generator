#include "itemsoverview.h"

ItemsOverview::ItemsOverview(ItemsTreeModel *model, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , treeView(new QTreeView())
{

    treeView->setModel(model);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);

    treeView->setAnimated(true);
    treeView->setRootIsDecorated(false);
    treeView->expandAll();

    setWidget(treeView);
    setWindowTitle(tr("Items overview"));

    show();

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ItemsOverview::onCurrentChanged);
}

void ItemsOverview::setModel(QAbstractItemModel *model)
{
    QItemSelectionModel *m = treeView->selectionModel();
    treeView->setModel(model);
    delete m;
}

void ItemsOverview::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(previous.isValid()) {
        static_cast<AbstractModelItem*>(previous.internalPointer())->setSelected(false);

    }
    if(current.isValid()) {
        AbstractModelItem *item =  static_cast<AbstractModelItem*>(current.internalPointer());
        item->setSelected(true);
        emit itemSelected(item->getSettingsLayout());
    } else {
        emit itemSelected(nullptr);
    }
}

void ItemsOverview::onSelectionChanged()
{
    treeView->selectionModel()->setCurrentIndex(findSelected(),
                                                QItemSelectionModel::ClearAndSelect);
}

QModelIndex ItemsOverview::findSelected(QModelIndex start)
{
    if(start.isValid()) {
        if(static_cast<AbstractModelItem*>(start.internalPointer())->isSelected()) {
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
