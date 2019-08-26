#ifndef TEXTUREEDITORWIDGET_H
#define TEXTUREEDITORWIDGET_H

#include "viewwidget.h"
#include "treemodel.h"
#include "texturegenerator.h"


class TextureEditorWidget : public QWidget
{
    Q_OBJECT
public:
    TextureEditorWidget(QWidget *parent = nullptr);

    ItemsTreeModel *getItemsTreeModel() const;
    TextureGenerator *getTextureGenerator() { return texGen; }

public slots:
    void addItem();

signals:
    void itemSelected(BasicItem *item);

private:
    ItemsTreeModel *treeModel;

    CanvasViewWidget *controllView;
    ImageViewWiget *diffuseView;
    ImageViewWiget *specularView;
    ImageViewWiget *normalView;

    TextureGenerator *texGen;

    void setUpLayout();
    void onItemSelected();

private slots:
    void updateImageViewers();
};

#endif // TEXTUREEDITORWIDGET_H
