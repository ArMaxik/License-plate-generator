#ifndef ITEMSTREEMODEL_H
#define ITEMSTREEMODEL_H

#include <QAbstractItemModel>
#include <QGraphicsScene>

#include "basicitem.h"

class ItemsTreeModel : public QAbstractItemModel
{
public:
    ItemsTreeModel(QGraphicsScene *gs, QObject *parent = nullptr);

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
private:
    QGraphicsScene *grScene;
};

#endif // ITEMSTREEMODEL_H
