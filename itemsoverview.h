#ifndef OBJECTSOVERVIEW_H
#define OBJECTSOVERVIEW_H

#include <QDockWidget>
#include <QTreeView>
#include <QGraphicsScene>

class ItemsOverview :public QDockWidget
{
    Q_OBJECT
public:
    ItemsOverview(QAbstractItemModel *model, QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

private:
    QTreeView *treeView;
};

#endif // OBJECTSOVERVIEW_H
