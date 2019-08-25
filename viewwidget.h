#ifndef CONTROLLVIEWPANEL_H
#define CONTROLLVIEWPANEL_H

#include "basicitem.h"

#include <QGraphicsScene>
#include <QGraphicsView>

class ScaleWidget;

class SceneViewWidget : public QWidget
{
    Q_OBJECT
public:
    SceneViewWidget(QWidget *parent = nullptr);

    void setCanvas(Canvas *c);
    void addItem(QGraphicsItem *item) { scene->addItem(item); }

    QGraphicsScene *getScene() const { return scene; }
    QGraphicsView *getView() const { return view; }

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Canvas *canvas;

    ScaleWidget *scaleW;

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void setScale(qreal scale);
    void recalulateSceneRect();
};

class ScaleWidget : public QWidget
{
Q_OBJECT
public:
    ScaleWidget(QWidget *parent = nullptr);

    qreal getScale() const { return scale; }

public slots:
//    void setScale(qreal s) { scale = s; }
    void increaseScale();
    void deincreaseScale();

signals:
    void scaleChanged(qreal s);

private:
    qreal scale;
    qreal scaleStep;

    QSpinBox *scaleSB;
};

#endif // CONTROLLVIEWPANEL_H
