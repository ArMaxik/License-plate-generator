#include "controllviewpanel.h"

#include <QtDebug>

ControllViewPanel::ControllViewPanel(QGraphicsScene *gs, QWidget *parent)
    : QGraphicsView (parent)
    , controllScene(gs)
{
    setScene(controllScene);

//    setMinimumSize(200, 200);
//    setMaximumSize(200, 200);

    controllScene->setSceneRect(0, 0, width(), height());
    setSceneRect(0, 0, width(), height());


}

void ControllViewPanel::setGraphicsScene(QGraphicsScene *gs)
{
    controllScene = gs;
}

