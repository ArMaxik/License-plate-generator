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

    QGraphicsScene *getScene() const { return scene; }
    QGraphicsView *getView() const { return view; }

protected:
    QGraphicsScene *scene;
    QGraphicsView *view;

    QRectF contentBound;

    ScaleWidget *scaleW;

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

protected slots:
    void setScale(qreal scale);
    virtual void recalulateSceneRect();
};

class CanvasViewWidget : public SceneViewWidget
{
    Q_OBJECT
public:
    CanvasViewWidget(QWidget *parent = nullptr);

    void setCanvas(Canvas *c);
    void addItem(QGraphicsItem *item) { scene->addItem(item); }

protected slots:
    void recalulateSceneRect() override;

private:
    Canvas *canvas;

};

class ImageViewWiget : public SceneViewWidget
{
    Q_OBJECT
public:
    ImageViewWiget(QWidget *parent = nullptr);

public slots:
    void setImage(const QImage *img);

private:
    QGraphicsPixmapItem *image;

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
