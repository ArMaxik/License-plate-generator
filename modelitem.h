#ifndef MODELITEM_H
#define MODELITEM_H

#include "basicitem.h"
#include "managers.h"

class AbstractModelItem
{
public:
    AbstractModelItem(AbstractModelItem *parent);
    ~AbstractModelItem();

    virtual void setSelected(bool selected) = 0;
    virtual std::unique_ptr<SmartLayout> &getSettingsLayout() = 0;
    virtual bool isSelected() = 0;
    virtual QVariant data(int column) const = 0;

    void appendChild(AbstractModelItem *child);
    AbstractModelItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    AbstractModelItem *parentItem();

protected:
    QVector<AbstractModelItem*> childItems;
    AbstractModelItem *parent;
};

class TreeItem : public AbstractModelItem
{
public:
    explicit TreeItem(BasicItem *item, AbstractModelItem *parent);

    QVariant data(int column) const override;

    BasicItem *getItem() const;

    void setSelected(bool selected) override;
    std::unique_ptr<SmartLayout> &getSettingsLayout() override;
    bool isSelected() override;

private:
    BasicItem *itemData;
};

class TreeManagerItem : public AbstractModelItem
{
public:
    TreeManagerItem(AbstractManager *item, AbstractModelItem *parent);

    QVariant data(int column) const override;

    AbstractManager *getItem() { return itemData; }

    void setSelected(bool selected) override;
    std::unique_ptr<SmartLayout> &getSettingsLayout() override;
    bool isSelected() override;

private:
    AbstractManager *itemData;
};

#endif // MODELITEM_H
