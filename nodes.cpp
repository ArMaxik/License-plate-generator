#include "nodes.h"

#include <QPainter>
#include <QSize>
#include <QFrame>

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
//    qDeleteAll(properites);
}

QLayout *BasicNode::getSettingsLayout()
{
    QVBoxLayout *ml = new QVBoxLayout();
//    QVBoxLayout *fl = new QVBoxLayout();

//    QFrame *frame = new QFrame();
//    frame->setLayout(fl);
//    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
//    frame->setLineWidth(1);

    foreach (BasicPropertie *p, properites) {
        QFrame *line;
        line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        ml->addWidget(line);
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

void BasicNode::toXml(QXmlStreamWriter &stream, int type)
{
    stream.writeStartElement("BasicNode");
    stream.writeAttribute("type", QString::number(type));
    foreach (BasicPropertie *p, properites) {
        p->toXml(stream);
    }
    stream.writeEndElement();
}

void BasicNode::randomize()
{
    foreach (BasicPropertie *p, properites) {
        p->randomize();
    }
}

void BasicNode::makeAllConnections()
{
    foreach (BasicPropertie *p, properites) {
        connect(p, &BasicPropertie::layoutChanged,
                this, &BasicNode::layoutChanged);
        connect(p, &BasicPropertie::changed,
                this, &BasicNode::changed);
    }
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

    connect(width, &NumberPropertie::changed,
            this, &ImageNode::changeSizeW);
    properites.push_back(width);

    connect(height, &NumberPropertie::changed,
            this, &ImageNode::changeSizeH);
    properites.push_back(height);
    makeAllConnections();
}

void ImageNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipping(true);
    QRectF r = bound->getBound();
    r.setSize(r.size() / scale);
    painter->setClipRect(r);
    painter->drawImage(0, 0, *image);
}

void ImageNode::setImageProperie(ImagePropertie *img)
{
    disconnect(image, &ImagePropertie::imageChange,
            this, &ImageNode::reloadImage);
    disconnect(image, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(image, &BasicPropertie::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(image);
    delete properites[i];
    properites.removeAt(i);

    image = img;
    connect(image, &ImagePropertie::imageChange,
            this, &ImageNode::reloadImage);
    connect(image, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(image, &BasicPropertie::changed,
            this, &BasicNode::changed);
    properites.push_back(image);
    reloadImage();
}

void ImageNode::setWidthProperie(NumberPropertie *w)
{
    disconnect(width, &NumberPropertie::changed,
            this, &ImageNode::changeSizeW);
    disconnect(width, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(width, &BasicPropertie::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(width);
    delete properites[i];
    properites.removeAt(i);

    width = w;
    connect(width, &NumberPropertie::changed,
            this, &ImageNode::changeSizeW);
    connect(width, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(width, &BasicPropertie::changed,
            this, &BasicNode::changed);
    properites.push_back(width);
    changeSizeW();
}

void ImageNode::setHeightPropertie(NumberPropertie *h)
{
    disconnect(height, &NumberPropertie::changed,
            this, &ImageNode::changeSizeW);
    disconnect(height, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(height, &BasicPropertie::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(height);
    delete properites[i];
    properites.removeAt(i);

    height = h;
    connect(height, &NumberPropertie::changed,
            this, &ImageNode::changeSizeH);
    connect(height, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(height, &BasicPropertie::changed,
            this, &BasicNode::changed);
    properites.push_back(height);
    changeSizeH();
}

void ImageNode::reloadImage()
{
    width->setValue(image->getImage().width());
    height->setValue(image->getImage().height());

    scale = 1.0;
//    emit scaleChanged(scale);

    if(affectSize) {
        bound->setSize(QSizeF(*width, *height));
    }
    emit changed();
}

void ImageNode::changeSizeW()
{
    int w = *width;
    scale = qreal(w) / qreal(image->getImage().width()) ;

    height->setValue(image->getImage().height() * scale, true);

    if(affectSize) {
        bound->setSize(QSizeF(*width, *height));
    }
    emit changed();
}

void ImageNode::changeSizeH()
{
    int h = *height;
    scale = qreal(h) / qreal(image->getImage().height());

    width->setValue(image->getImage().width() * scale, true);

    if(affectSize) {
        bound->setSize(QSizeF(*width, *height));
    }
    emit changed();
}

// ========[ TextNode ]==================================================

TextNode::TextNode(BoundRect *br)
    : BasicNode (br)
    , string(new StringPropertie(tr("Text")))
    , fontSize(new NumberPropertie(tr("Text size"), 20, 1, 10000))
    , color(new ColorPropertie(tr("Text Color")))
{
    connect(string, &StringPropertie::stringChange,
            this, &TextNode::stringChanged);
    properites.push_back(string);

    font.setPixelSize(20);
    connect(fontSize, &NumberPropertie::changed,
            this, [this] () { font.setPixelSize(*fontSize); updateBound(); emit changed(); });
    properites.push_back(fontSize);

    properites.push_back(color);
    connect(color, &BasicPropertie::changed,
            this, &BasicNode::changed);
    updateBound();
    makeAllConnections();
}


void TextNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setFont(font);
    painter->setPen(*color);

//    if(updateBound && affectSize) {
//        QRectF newR = painter->boundingRect(bound->getBound(), Qt::AlignLeft, *string);
//        bound->setSize(newR.size());
//    }
    painter->drawText(bound->getBound(), *string);
}

void TextNode::setStringProperie(StringPropertie *str)
{
    disconnect(string, &StringPropertie::stringChange,
            this, &TextNode::stringChanged);
    disconnect(string, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(string, &BasicPropertie::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(string);
    delete properites[i];
    properites.removeAt(i);

    string = str;
    connect(string, &StringPropertie::stringChange,
            this, &TextNode::stringChanged);
    connect(string, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(string, &BasicPropertie::changed,
            this, &BasicNode::changed);
    properites.push_back(string);
    updateBound();
}

void TextNode::setFontSizeProperie(NumberPropertie *size)
{
    disconnect(fontSize, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(fontSize, &BasicPropertie::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(fontSize);
    delete properites[i];
    properites.removeAt(i);

    fontSize = size;
    connect(fontSize, &NumberPropertie::changed,
            this, [this] () { font.setPixelSize(*fontSize); updateBound(); emit changed(); });
    connect(fontSize, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(fontSize, &BasicPropertie::changed,
            this, &BasicNode::changed);
    properites.push_back(fontSize);
    font.setPixelSize(*fontSize);
    updateBound();
}

void TextNode::setColortPropertie(ColorPropertie *c)
{
    disconnect(color, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(color, &BasicPropertie::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(color);
    delete properites[i];
    properites.removeAt(i);

    color = c;
    connect(color, &BasicPropertie::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(color, &BasicPropertie::changed,
            this, &BasicNode::changed);
    properites.push_back(color);
    updateBound();
}

void TextNode::updateBound()
{
    if(affectSize) {
        QSize imgS;
        QRectF currBound = bound->getBound();
        if(currBound.width() == 0) {
            imgS = QSize(500, 500);
        } else {
            imgS = QSize(bound->getBound().width() * 1.5,
                         bound->getBound().height());
        }

        QPixmap img = QPixmap(imgS);
        QPainter p(&img);
        p.setFont(font);
        QRectF newR = p.boundingRect(bound->getBound(), Qt::AlignLeft, *string);
        bound->setSize(newR.size());
    }
}

void TextNode::stringChanged(QString newStr)
{
//    updateBound = true;
    updateBound();
    emit changed();
}

// ========[ FillBasckgroundNode ]==================================================

FillBasckgroundNode::FillBasckgroundNode(BoundRect *br, QColor defaultColor)
    : BasicNode(br)
    , color(new ColorPropertie(tr("Fill color"), defaultColor))
{
    properites.push_back(color);
    connect(color, &BasicPropertie::changed,
            this, &BasicNode::changed);
    makeAllConnections();
}

void FillBasckgroundNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(*color);
    painter->setBrush(QBrush(*color));
    painter->drawRect(bound->getBound());
}
