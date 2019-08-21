#ifndef IMAGETEXTURE_H
#define IMAGETEXTURE_H

#include <Qt3DRender/QPaintedTextureImage>

#include <QImage>
#include <QPen>
#include <QBrush>
#include <QPainter>

class ImageTexture : public Qt3DRender::QPaintedTextureImage
{
    Q_OBJECT
public:
    ImageTexture(Qt3DCore::QNode *parent = nullptr, QImage *_img = nullptr) :
        QPaintedTextureImage(parent)
    {
        image = _img;
        setSize(QSize(280, 140));
    }

public slots:
    void setImage(QImage *_image)
    {
        QImage *buf = image;
        image = _image;
        delete  buf;
        setSize(image->size());

        update();
    }

protected:
    void paint(QPainter *painter) override {
        if(image == nullptr)
        {   //  Черный цвет, чтобы не было глянца
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
            painter->drawRect(0, 0, size().width(), size().height());
        } else {
            painter->drawImage(0, 0, *image);
        }
    }

private:
    QImage *image;
};

#endif // IMAGETEXTURE_H
