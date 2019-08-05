#ifndef TEXTUREEDITORWIDGET_H
#define TEXTUREEDITORWIDGET_H

#include "customcentralwidget.h"
#include "controllviewpanel.h"

class TextureEditorWidget : public CustomCentralWidget
{
    Q_OBJECT
public:
    TextureEditorWidget(QWidget *parent = nullptr);

public slots:
    void addStaticImageItem();

signals:
    void itemSelected(BasicItem *item);

private:
    QGraphicsScene *controllScene;
    ControllViewPanel *controllView;

    void setUpLayout();
    void onItemSelected();

};

#endif // TEXTUREEDITORWIDGET_H
