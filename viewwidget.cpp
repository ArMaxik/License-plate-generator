#include "viewwidget.h"

#include <QtDebug>
#include <QKeyEvent>

#define BACK_BORDER 50

ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent)
    , scene(new QGraphicsScene())
    , view(new QGraphicsView(scene))
    , canvas(nullptr)
    , scale(1.0)
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

void ViewWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        if(event->key() == Qt::Key_Plus) {
            view->scale((scale+0.1)/scale, (scale+0.1)/scale);
            scale += 0.1;
            recalulateSceneRect();
        }
        if(event->key() == Qt::Key_Minus) {
            view->scale((scale-0.1)/scale, (scale-0.1)/scale);
            scale -= 0.1;
            recalulateSceneRect();
        }
    }
}

void ViewWidget::recalulateSceneRect()
{

    if(canvas != nullptr) {
        QRect content = view->contentsRect();
        QRectF sceneR = canvas->boundingRect();
        if(sceneR.width() + BACK_BORDER*2 < content.width()) {
            sceneR.setX(-(content.width() - sceneR.width())  / 2);
            sceneR.setWidth(content.width());
        } else {
            sceneR.adjust(-BACK_BORDER, 0.0, BACK_BORDER, 0.0);
        }
        if(sceneR.height() + BACK_BORDER*2 < content.height()) {
            sceneR.setY(-(content.height() - sceneR.height())  / 2);
            sceneR.setHeight(content.height());
        } else {
            sceneR.adjust(0.0, -BACK_BORDER, 0.0, BACK_BORDER);
        }
        scene->setSceneRect(sceneR);
    }
}

