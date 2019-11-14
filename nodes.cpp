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

    foreach (BasicProperty *p, properites) {
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
    foreach (BasicProperty *p, properites) {
        p->toXml(stream);
    }
    stream.writeEndElement();
}

void BasicNode::randomize()
{
    foreach (BasicProperty *p, properites) {
        p->randomize();
    }
}

void BasicNode::makeAllConnections()
{
    foreach (BasicProperty *p, properites) {
        connect(p, &BasicProperty::layoutChanged,
                this, &BasicNode::layoutChanged);
        connect(p, &BasicProperty::changed,
                this, &BasicNode::changed);
    }
}

// ========[ ImageNode ]==================================================

ImageNode::ImageNode(BoundRect *br)
    : BasicNode(br)
    , image(new ImageProperty(tr("Image")))
    , width(new NumberProperty(tr("Image Width"), 0, 0, 10000))
    , height(new NumberProperty(tr("Image Height"), 0, 0, 10000))
{
    connect(image, &ImageProperty::imageChange,
            this, &ImageNode::reloadImage);
    properites.push_back(image);

    connect(width, &NumberProperty::changed,
            this, &ImageNode::changeSizeW);
    properites.push_back(width);

    connect(height, &NumberProperty::changed,
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

void ImageNode::setImageProperty(ImageProperty *img)
{
    disconnect(image, &ImageProperty::imageChange,
            this, &ImageNode::reloadImage);
    disconnect(image, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(image, &BasicProperty::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(image);
    delete properites[i];
    properites.removeAt(i);

    image = img;
    connect(image, &ImageProperty::imageChange,
            this, &ImageNode::reloadImage);
    connect(image, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(image, &BasicProperty::changed,
            this, &BasicNode::changed);
    properites.push_back(image);
    reloadImage();
}

void ImageNode::setWidthProperty(NumberProperty *w)
{
    disconnect(width, &NumberProperty::changed,
            this, &ImageNode::changeSizeW);
    disconnect(width, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(width, &BasicProperty::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(width);
    delete properites[i];
    properites.removeAt(i);

    width = w;
    connect(width, &NumberProperty::changed,
            this, &ImageNode::changeSizeW);
    connect(width, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(width, &BasicProperty::changed,
            this, &BasicNode::changed);
    properites.push_back(width);
    changeSizeW();
}

void ImageNode::setHeightPropertie(NumberProperty *h)
{
    disconnect(height, &NumberProperty::changed,
            this, &ImageNode::changeSizeW);
    disconnect(height, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(height, &BasicProperty::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(height);
    delete properites[i];
    properites.removeAt(i);

    height = h;
    connect(height, &NumberProperty::changed,
            this, &ImageNode::changeSizeH);
    connect(height, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(height, &BasicProperty::changed,
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
    , string(new StringProperty(tr("Text")))
    , fontSize(new NumberProperty(tr("Text size"), 20, 1, 10000))
    , color(new ColorProperty(tr("Text Color")))
{
    connect(string, &StringProperty::stringChange,
            this, &TextNode::stringChanged);
    properites.push_back(string);

    font.setPixelSize(20);
    connect(fontSize, &NumberProperty::changed,
            this, [this] () { font.setPixelSize(*fontSize); updateBound(); emit changed(); });
    properites.push_back(fontSize);

    properites.push_back(color);
    connect(color, &BasicProperty::changed,
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

void TextNode::setStringProperty(StringProperty *str)
{
    disconnect(string, &StringProperty::stringChange,
            this, &TextNode::stringChanged);
    disconnect(string, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(string, &BasicProperty::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(string);
    delete properites[i];
    properites.removeAt(i);

    string = str;
    connect(string, &StringProperty::stringChange,
            this, &TextNode::stringChanged);
    connect(string, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(string, &BasicProperty::changed,
            this, &BasicNode::changed);
    properites.push_back(string);
    updateBound();
}

void TextNode::setFontSizeProperty(NumberProperty *size)
{
    disconnect(fontSize, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(fontSize, &BasicProperty::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(fontSize);
    delete properites[i];
    properites.removeAt(i);

    fontSize = size;
    connect(fontSize, &NumberProperty::changed,
            this, [this] () { font.setPixelSize(*fontSize); updateBound(); emit changed(); });
    connect(fontSize, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(fontSize, &BasicProperty::changed,
            this, &BasicNode::changed);
    properites.push_back(fontSize);
    font.setPixelSize(*fontSize);
    updateBound();
}

void TextNode::setColortPropertie(ColorProperty *c)
{
    disconnect(color, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    disconnect(color, &BasicProperty::changed,
            this, &BasicNode::changed);

    int i = properites.indexOf(color);
    delete properites[i];
    properites.removeAt(i);

    color = c;
    connect(color, &BasicProperty::layoutChanged,
            this, &BasicNode::layoutChanged);
    connect(color, &BasicProperty::changed,
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
    , color(new ColorProperty(tr("Fill color"), defaultColor))
{
    properites.push_back(color);
    connect(color, &BasicProperty::changed,
            this, &BasicNode::changed);
    makeAllConnections();
}

void FillBasckgroundNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(*color);
    painter->setBrush(QBrush(*color));
    painter->drawRect(bound->getBound());
}
