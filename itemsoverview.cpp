#include "itemsoverview.h"
#include "itemstreemodel.h"

ItemsOverview::ItemsOverview(QAbstractItemModel *model, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget()
    , treeView(new QTreeView())
{

//    ItemsTreeModel *model = new ItemsTreeModel(gs);
    treeView->setModel(model);

    setWidget(treeView);
    setWindowTitle(tr("Items overview"));

    show();
}
