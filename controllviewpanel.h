#ifndef CONTROLLVIEWPANEL_H
#define CONTROLLVIEWPANEL_H

#include "basicitem.h"

#include <QGraphicsScene>
#include <QGraphicsView>

class ControllViewPanel : public QGraphicsView
{
    Q_OBJECT
public:
    ControllViewPanel(QGraphicsScene *gs, QWidget *parent = nullptr);

    void setGraphicsScene(QGraphicsScene *gs);



private:
    QGraphicsScene *controllScene;

};

#endif // CONTROLLVIEWPANEL_H
