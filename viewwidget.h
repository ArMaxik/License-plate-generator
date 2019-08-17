#ifndef CONTROLLVIEWPANEL_H
#define CONTROLLVIEWPANEL_H

#include "basicitem.h"

#include <QGraphicsScene>
#include <QGraphicsView>

class ViewWidget : public QWidget
{
    Q_OBJECT
public:
    ViewWidget(QWidget *parent = nullptr);

    void setGraphicsScene(QGraphicsScene *gs);
    void setCanvas(Canvas *c);
    void addItem(QGraphicsItem *item) { scene->addItem(item); }

    QGraphicsScene *getScene() const { return scene; }
    QGraphicsView *getView() const { return view; }

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Canvas *canvas;
    qreal scale;

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void recalulateSceneRect();
};

#endif // CONTROLLVIEWPANEL_H
