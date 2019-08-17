#ifndef TEXTUREEDITORWIDGET_H
#define TEXTUREEDITORWIDGET_H

#include "customcentralwidget.h"
#include "viewwidget.h"
#include "itemstreemodel.h"

class TextureEditorWidget : public CustomCentralWidget
{
    Q_OBJECT
public:
    TextureEditorWidget(QWidget *parent = nullptr);

    QGraphicsScene *getControllScene() const;
    ItemsTreeModel *getItemsTreeModel() const;

public slots:
    void addItem();
//    void addStaticImageItem();
//    void addStaticTextItem();

signals:
    void itemSelected(BasicItem *item);

private:
    ItemsTreeModel *treeModel;

    QGraphicsScene *controllScene;
    ViewWidget *controllView;

    QGraphicsScene *diffuseScene;
    ViewWidget *diffuseView;

    QGraphicsScene *specularScene;
    ViewWidget *specularView;

    QGraphicsScene *normalScene;
    ViewWidget *normalView;

    void setUpLayout();
    void onItemSelected();

private slots:

};

#endif // TEXTUREEDITORWIDGET_H
