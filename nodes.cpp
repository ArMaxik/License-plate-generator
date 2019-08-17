#include "nodes.h"

#include <QPainter>
#include <QSize>

#include <QtDebug>

// ========[ BasicNode ]==================================================
BasicNode::BasicNode(BoundRect *br)
    : bound(br)
    , affectSize(false)
    , scale(1.0)
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

// ========[ ImageNode ]==================================================

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
    painter->setClipping(true);
    QRectF r = bound->getBound();
    r.setSize(r.size() / scale);
    painter->setClipRect(r);
    painter->drawImage(0, 0, *image);
}

void ImageNode::reloadImage()
{
    width->setValue(image->getImage().width());
    height->setValue(image->getImage().height());

    scale = 1.0;
    emit scaleChanged(scale);

    if(affectSize) {
        bound->setSize(QSizeF(*width, *height));
    }
}

void ImageNode::changeSizeW(int w)
{
    scale = qreal(w) / qreal(image->getImage().width()) ;
    emit scaleChanged(scale);
    height->setValue(image->getImage().height() * scale);

    if(affectSize) {
        bound->setSize(QSizeF(*width, *height));
    }
}

void ImageNode::changeSizeH(int h)
{
    scale = qreal(h) / qreal(image->getImage().height());
    emit scaleChanged(scale);
    width->setValue(image->getImage().width() * scale);

    if(affectSize) {
        bound->setSize(QSizeF(*width, *height));
    }
}

// ========[ TextNode ]==================================================

TextNode::TextNode(BoundRect *br)
    : BasicNode (br)
    , string(new StringPropertie(tr("Text")))
    , fontSize(new NumberPropertie(tr("Text size"), 20, 1, 10000))
    , color(new ColorPropertie(tr("Text Color")))
    , updateBound(true)
{
    connect(string, &StringPropertie::stringChange,
            this, &TextNode::stringChanged);
    properites.push_back(string);

    font.setPixelSize(20);
    connect(fontSize, &NumberPropertie::numberValueChange,
            this, [this](int size) { font.setPixelSize(size); });
    properites.push_back(fontSize);

    properites.push_back(color);
}


void TextNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setFont(font);
    painter->setPen(*color);

    if(updateBound && affectSize) {
        QRectF newR = painter->boundingRect(bound->getBound(), Qt::AlignLeft, *string);
        bound->setSize(newR.size());
    }
    painter->drawText(bound->getBound(), *string);
}

void TextNode::stringChanged(QString newStr)
{
    updateBound = true;
}
