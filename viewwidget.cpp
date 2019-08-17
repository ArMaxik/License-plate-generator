#include "viewwidget.h"

#include <QtDebug>

ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent)
    , scene(new QGraphicsScene())
    , view(new QGraphicsView(scene))
    , canvas(nullptr)
{
    QVBoxLayout *lo = new QVBoxLayout();
    lo->addWidget(view);
    lo->setMargin(7);
    setLayout(lo);

    scene->setBackgroundBrush(QBrush(Qt::lightGray));


//    view->setSceneRect(0, 0, width(), height());


}

void ViewWidget::setGraphicsScene(QGraphicsScene *gs)
{
    scene = gs;
}

void ViewWidget::setCanvas(Canvas *c)
{
    if(canvas != nullptr) {
        disconnect(canvas->getBoundRect(), &BoundRect::sizeChanged,
                this, &ViewWidget::recalulateSceneRect);
    }
    canvas = c;
    connect(canvas->getBoundRect(), &BoundRect::sizeChanged,
            this, &ViewWidget::recalulateSceneRect);
    recalulateSceneRect();
}

void ViewWidget::resizeEvent(QResizeEvent *event)
{
    recalulateSceneRect();
}

void ViewWidget::recalulateSceneRect()
{

    if(canvas != nullptr){
        QRect content = view->contentsRect();
        QRectF sceneR = canvas->boundingRect();
        if(sceneR.width() < content.width()) {
            sceneR.setX(-(content.width() - sceneR.width())  / 2);
            sceneR.setWidth(content.width());
        }
        if(sceneR.height() < content.height()) {
            sceneR.setY(-(content.height() - sceneR.height())  / 2);
            sceneR.setHeight(content.height());
        }
        scene->setSceneRect(sceneR);
    }
}

