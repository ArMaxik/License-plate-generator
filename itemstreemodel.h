#ifndef ITEMSTREEMODEL_H
#define ITEMSTREEMODEL_H

#include <QAbstractItemModel>
#include <QGraphicsScene>

#include "basicitem.h"

class TreeItem
{
public:
    explicit TreeItem(BasicItem *item, TreeItem *parent);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QVector<TreeItem*> m_childItems;
    BasicItem *m_itemData;
    TreeItem *m_parentItem;
};

class ItemsTreeModel : public QAbstractItemModel
{
public:
    ItemsTreeModel(QObject *parent = nullptr);

    void addItem(BasicItem *item);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::DropActions supportedDragActions() const override;
private:
    TreeItem *rootItem;
};

#endif // ITEMSTREEMODEL_H
