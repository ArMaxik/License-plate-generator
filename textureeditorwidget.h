#ifndef TEXTUREEDITORWIDGET_H
#define TEXTUREEDITORWIDGET_H

#include "customcentralwidget.h"
#include "viewwidget.h"
#include "treemodel.h"


class TextureEditorWidget : public QWidget
{
    Q_OBJECT
public:
    TextureEditorWidget(QWidget *parent = nullptr);

    ItemsTreeModel *getItemsTreeModel() const;

public slots:
    void addItem();

signals:
    void itemSelected(BasicItem *item);

private:
    ItemsTreeModel *treeModel;

    ViewWidget *controllView;
    ViewWidget *diffuseView;
    ViewWidget *specularView;
    ViewWidget *normalView;

    void setUpLayout();
    void onItemSelected();

private slots:

};

#endif // TEXTUREEDITORWIDGET_H
