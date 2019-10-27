#ifndef ITEMSTREEMODEL_H
#define ITEMSTREEMODEL_H

#include <QAbstractItemModel>
#include <QGraphicsScene>

#include "modelitem.h"

class AbstractTreeModel : public QAbstractItemModel
{
public:
    AbstractTreeModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::DropActions supportedDragActions() const override;

protected:
    AbstractModelItem *rootItem;
};


class ItemsTreeModel : public AbstractTreeModel
{
public:
    ItemsTreeModel(QObject *parent = nullptr);

    void addItem(BasicItem *item);
    void setCanvas(Canvas *c);

public slots:
    void clear();

private:
    TreeItem *canvas;
};

class SceneTreeModel : public AbstractTreeModel
{
public:
    SceneTreeModel(QObject *parent = nullptr);

    void addItem(AbstractManager *item);
};

#endif // ITEMSTREEMODEL_H
