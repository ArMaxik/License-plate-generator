#ifndef OBJECTSOVERVIEW_H
#define OBJECTSOVERVIEW_H

#include "itemstreemodel.h"

#include <QDockWidget>
#include <QTreeView>
#include <QGraphicsScene>

class ItemsOverview :public QDockWidget
{
    Q_OBJECT
public:
    ItemsOverview(ItemsTreeModel *model, QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

private:
    QTreeView *treeView;

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSelectionChanged();
    QModelIndex findSelected(QModelIndex start = QModelIndex());
};

#endif // OBJECTSOVERVIEW_H
