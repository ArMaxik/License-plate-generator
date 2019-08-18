#include "viewwidget.h"

#include <QtDebug>
#include <QKeyEvent>
#include <QStatusBar>

#define BACK_BORDER 50

ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent)
    , scene(new QGraphicsScene())
    , view(new QGraphicsView(scene))
    , canvas(nullptr)
    , scaleW(new ScaleWidget())
{
    QVBoxLayout *lo = new QVBoxLayout();
    QStatusBar *sb = new QStatusBar();

    sb->addPermanentWidget(scaleW);
    sb->setSizeGripEnabled(false);

    lo->addWidget(view);
    lo->addWidget(sb);
    lo->setMargin(7);
    setLayout(lo);

    scene->setBackgroundBrush(QBrush(Qt::lightGray));


    connect(scaleW, &ScaleWidget::scaleChanged,
            this, &ViewWidget::setScale);


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
            scaleW->increaseScale();
        }
        if(event->key() == Qt::Key_Minus) {
            scaleW->deincreaseScale();
        }
    }
}

void ViewWidget::setScale(qreal scale)
{
    view->scale(scale, scale);
    recalulateSceneRect();
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


ScaleWidget::ScaleWidget(QWidget *parent)
    : QWidget (parent)
    , scale(1.0)
    , scaleStep(0.1)
{
    QHBoxLayout *ml = new QHBoxLayout();
    scaleSB = new QSpinBox();

    scaleSB->setRange(10, 1000);
    scaleSB->setSuffix(tr("%"));
    scaleSB->setSingleStep(scaleStep * 100);
    scaleSB->setValue(scale * 100);

    ml->addWidget(scaleSB);
    ml->setMargin(0);
    setLayout(ml);

    connect(scaleSB, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int s){ emit scaleChanged((s / 100.0)/scale); scale=s/100.0; });
}

void ScaleWidget::increaseScale()
{
    scaleSB->setValue(scale + scaleStep);
}

void ScaleWidget::deincreaseScale()
{
    scaleSB->setValue(scale - scaleStep);
}
