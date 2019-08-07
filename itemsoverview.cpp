#include "itemsoverview.h"

ItemsOverview::ItemsOverview(ItemsTreeModel *model, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , treeView(new QTreeView())
{

//    ItemsTreeModel *model = new ItemsTreeModel(gs);
    treeView->setModel(model);

    setWidget(treeView);
    setWindowTitle(tr("Items overview"));

    show();

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ItemsOverview::onCurrentChanged);
    connect(model->getGraphicsScene(), &QGraphicsScene::selectionChanged,
            this, &ItemsOverview::onSelectionChanged);
}

void ItemsOverview::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(previous.isValid())
        static_cast<BasicItem*>(previous.internalPointer())->setSelected(false);
    if(current.isValid())
        static_cast<BasicItem*>(current.internalPointer())->setSelected(true);
}

void ItemsOverview::onSelectionChanged()
{
    treeView->selectionModel()->setCurrentIndex(findSelected(),
                                                QItemSelectionModel::ClearAndSelect);
}

QModelIndex ItemsOverview::findSelected(QModelIndex start)
{
    if(start.isValid()) {
        if(static_cast<BasicItem*>(start.internalPointer())->isSelected()) {
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
