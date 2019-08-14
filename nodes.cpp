#include "nodes.h"

#include <QPainter>
#include <QSize>

#include <QtDebug>

BasicNode::BasicNode(BoundRect *br)
    : bound(br)
{

}

BasicNode::~BasicNode()
{
    qDeleteAll(properites);
}

QLayout *BasicNode::getSettingsLayout()
{
    QVBoxLayout *ml = new QVBoxLayout();
//    QVBoxLayout *fl = new QVBoxLayout();

//    QFrame *frame = new QFrame();
//    frame->setLayout(fl);
//    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
//    frame->setLineWidth(1);

    foreach (const BasicPropertie *p, properites) {
        ml->addLayout(p->getSettingsLayout());
    }

//    ml->addWidget(frame);

    return ml;
}

void BasicNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    return QSize(0, 0);
//    qDebug() <<"BasicNode " << boundingRect();

}

ImageNode::ImageNode(BoundRect *br)
    : BasicNode(br)
    , image(new ImagePropertie(tr("Image")))
    , width(new NumberPropertie(tr("Image Width"), 0, 0, 10000))
    , height(new NumberPropertie(tr("Image Height"), 0, 0, 10000))
{
    connect(image, &ImagePropertie::imageChange,
            this, &ImageNode::reloadImage);
    properites.push_back(image);

    connect(width, &NumberPropertie::numberValueChange,
            this, &ImageNode::changeSizeW);
    properites.push_back(width);

    connect(height, &NumberPropertie::numberValueChange,
            this, &ImageNode::changeSizeH);
    properites.push_back(height);
}

void ImageNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(0, 0, *image);
}

void ImageNode::reloadImage()
{
    width->setValue(image->getImage().width());
    height->setValue(image->getImage().height());
    bound->setSize(QSizeF(*width, *height));
}

void ImageNode::changeSizeW(int w)
{
    qreal factor = qreal(w) / qreal(image->getImage().width()) ;
    bound->setScale(factor);
    height->setValue(image->getImage().height() * factor);
}

void ImageNode::changeSizeH(int h)
{
    qreal factor = qreal(h) / qreal(image->getImage().height());
    bound->setScale(factor);
    width->setValue(image->getImage().width() * factor);
}

