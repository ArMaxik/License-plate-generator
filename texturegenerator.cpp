#include "texturegenerator.h"

#include <QPainter>

TextureGenerator::TextureGenerator(QObject *parent)
    : QObject (parent)
    , diffuseTextureScene(new QGraphicsScene)
    , specularTextureScene(new QGraphicsScene)
    , normalTextureScene(new QGraphicsScene)
{
}

QImage *TextureGenerator::getDiffuseTexture()
{
    QImage *res = new QImage(canvas->boundingRect().width(),
                             canvas->boundingRect().height(),
                             QImage::Format_ARGB32);
    QPainter p(res);

    diffuseTextureScene->render(&p, canvas->boundingRect(), canvas->boundingRect());

    return res;
}

QImage *TextureGenerator::getSpecularTexture()
{
    QImage *res = new QImage(canvas->boundingRect().width(),
                             canvas->boundingRect().height(),
                             QImage::Format_ARGB32);
    QPainter p(res);

    specularTextureScene->render(&p, canvas->boundingRect(), canvas->boundingRect());

    return res;
}

QImage *TextureGenerator::getNormalTexture()
{
    QImage *res = new QImage(canvas->boundingRect().width(),
                             canvas->boundingRect().height(),
                             QImage::Format_ARGB32);
    QPainter p(res);

    normalTextureScene->render(&p, canvas->boundingRect(), canvas->boundingRect());

    return res;
}

void TextureGenerator::setCanvas(Canvas *c)
{
    diffuseTextureScene->addItem(c->getDiffuseChanel());
    specularTextureScene->addItem(c->getSpecularChanel());
    normalTextureScene->addItem(c->getNormalChanel());
    canvas = c;
}

void TextureGenerator::randomize()
{
    randomizeChildren(canvas->getDiffuseChanel());
    randomizeChildren(canvas->getSpecularChanel());
    randomizeChildren(canvas->getNormalChanel());
}

void TextureGenerator::randomizeChildren(QGraphicsItem *parent)
{
    for(QGraphicsItem *i :parent->childItems()) {
        static_cast<BasicChanel*>(i)->randomize();
        randomizeChildren(i);
    }
}
