#ifndef TEXTUREEDITORWIDGET_H
#define TEXTUREEDITORWIDGET_H

#include "customcentralwidget.h"
#include "controllviewpanel.h"
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
    ControllViewPanel *controllView;

    QGraphicsScene *diffuseScene;
    ControllViewPanel *diffuseView;

    QGraphicsScene *specularScene;
    ControllViewPanel *specularView;

    QGraphicsScene *normalScene;
    ControllViewPanel *normalView;

    void setUpLayout();
    void onItemSelected();

private slots:

};

#endif // TEXTUREEDITORWIDGET_H
