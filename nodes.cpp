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

    foreach (const BasicPropertie *p, properites) {
        ml->addLayout(p->getSettingsLayout());
    }

    return ml;
}

void BasicNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    return QSize(0, 0);
//    qDebug() <<"BasicNode " << boundingRect();

}

ImageNode::ImageNode(BoundRect *br)
    : BasicNode(br)
    , image("C:/Users/slava/OneDrive/SOavater.png")  // Временно, потом убери
    , width(new NumberPropertie(tr("Image Width"), 0, 0, 10000))
    , height(new NumberPropertie(tr("Image Height"), 0, 0, 10000))
{
    ImagePropertie *imgPr = new ImagePropertie(tr("Image Path"));
    connect(imgPr, &ImagePropertie::imagePathChange,
            this, &ImageNode::reloadImage);
    properites.push_back(imgPr);

    connect(width, &NumberPropertie::numberValueChange,
            this, &ImageNode::changeSizeW);
    properites.push_back(width);

    connect(height, &NumberPropertie::numberValueChange,
            this, &ImageNode::changeSizeH);
    properites.push_back(height);
}

void ImageNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(0, 0, image);
}

void ImageNode::reloadImage(QString newPath)
{
    image = QImage(newPath);
    width->setValue(image.width());
    height->setValue(image.height());
//    emit boundSizeChanged(QSizeF(*width, *height));
    bound->setSize(QSizeF(*width, *height));
}

void ImageNode::changeSizeW(int w)
{
    qreal factor = qreal(w) / qreal(image.width()) ;
//    emit scaleChanged(factor);
    bound->setScale(factor);
    height->setValue(image.height() * factor);
}

void ImageNode::changeSizeH(int h)
{
    qreal factor = qreal(h) / qreal(image.height());
//    emit scaleChanged(factor);
    bound->setScale(factor);
    width->setValue(image.width() * factor);
}

