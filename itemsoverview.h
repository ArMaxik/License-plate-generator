#ifndef OBJECTSOVERVIEW_H
#define OBJECTSOVERVIEW_H

#include "treemodel.h"

#include <QDockWidget>
#include <QTreeView>
#include <QGraphicsScene>

class ItemsOverview :public QDockWidget
{
    Q_OBJECT
public:
    ItemsOverview(ItemsTreeModel *model, QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

    void setModel(QAbstractItemModel *model);
    void setSelectionModel(QItemSelectionModel *selectionModel);

public slots:
    void onSelectionChanged();

signals:
    void itemSelected(QLayout *layout);

private:
    QTreeView *treeView;

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    QModelIndex findSelected(QModelIndex start = QModelIndex());
};

#endif // OBJECTSOVERVIEW_H
