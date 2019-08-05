#include "basicitem.h"

#include <QPainter>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>

#include<QDebug>


BasicItem::BasicItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , bound(0, 0, 50, 50)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}  // Что-то тут не то, хммм

void BasicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected()) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        painter->setPen(pen);

        painter->drawRect(boundingRect());
    }
}

QLayout *BasicItem::getSettingsLayout()
{
//    QVBoxLayout *vl = new QVBoxLayout();
    QGridLayout *gl = new QGridLayout();

    QCheckBox *movableCB = new QCheckBox();
    movableCB->setCheckState(Qt::Checked);

    QSpinBox *xSpinBox = new QSpinBox();
    QSpinBox *ySpinBox = new QSpinBox();
    xSpinBox->setRange(0, 5000);
    ySpinBox->setRange(0, 5000);
    xSpinBox->setValue(x());
    ySpinBox->setValue(y());

    QSpinBox *wSpinBox = new QSpinBox();
    QSpinBox *hSpinBox = new QSpinBox();
    wSpinBox->setRange(0, 5000);
    hSpinBox->setRange(0, 5000);
    wSpinBox->setValue(boundingRect().width());
    hSpinBox->setValue(boundingRect().height());

    QLabel *movL = new QLabel(tr("Movable"));
    QLabel *xposL = new QLabel(tr("X position"));
    QLabel *yposL = new QLabel(tr("Y position"));
    QLabel *widthL = new QLabel(tr("Width"));
    QLabel *heightL = new QLabel(tr("Height"));

    gl->addWidget(movL, 0, 0);
    gl->addWidget(movableCB, 0, 1);
    gl->addWidget(xposL, 1, 0);
    gl->addWidget(xSpinBox, 1, 1);
    gl->addWidget(yposL, 2, 0);
    gl->addWidget(ySpinBox, 2, 1);
    gl->addWidget(widthL, 3, 0);
    gl->addWidget(wSpinBox, 3, 1);
    gl->addWidget(heightL, 4, 0);
    gl->addWidget(hSpinBox, 4, 1);

    // Movable check
    connect(movableCB, &QCheckBox::stateChanged,
            this, [this](int state){ setFlag(QGraphicsItem::ItemIsMovable, state); });
    connect(movableCB, &QCheckBox::stateChanged,
            xSpinBox, &QSpinBox::setEnabled);
    connect(movableCB, &QCheckBox::stateChanged,
            ySpinBox, &QSpinBox::setEnabled);
    // Position spinBox affect on object
    connect(xSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int x){ setX(x); });
    connect(ySpinBox,  QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int y){ setY(y); });
    // Object position affect in spinBox
    connect(this, &QGraphicsObject::xChanged,
            xSpinBox, [this, xSpinBox](){xSpinBox->setValue(x());});
    connect(this, &QGraphicsObject::yChanged,
            ySpinBox, [this, ySpinBox](){ySpinBox->setValue(y());});
    // spinBox size
    connect(wSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int w){ prepareGeometryChange(); bound.setWidth(w);});
    connect(hSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int h){ prepareGeometryChange(); bound.setHeight(h);});

    return  gl;
}

QRectF BasicItem::boundingRect() const
{
    return bound;
}

StaticImageItem::StaticImageItem(QGraphicsItem *parent)
    : BasicItem (parent)
{

}

QLayout *StaticImageItem::getSettingsLayout()
{
    return BasicItem::getSettingsLayout();
}

void StaticImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
    painter->drawEllipse(boundingRect());
    BasicItem::paint(painter, option, widget);
}


