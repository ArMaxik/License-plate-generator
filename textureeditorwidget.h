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
    void addStaticImageItem();
    void addStaticTextItem();

signals:
    void itemSelected(BasicItem *item);

private:
    QGraphicsScene *controllScene;
    ControllViewPanel *controllView;
    ItemsTreeModel *treeModel;

    void setUpLayout();
    void onItemSelected();

private slots:

};

#endif // TEXTUREEDITORWIDGET_H
