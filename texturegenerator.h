#ifndef TEXTUREGENERATOR_H
#define TEXTUREGENERATOR_H

#include "basicitem.h"

#include <QGraphicsScene>

class TextureGenerator : public QObject
{
    Q_OBJECT
public:
    TextureGenerator(QObject *parent = nullptr);

    QImage *getDiffuseTexture();
    QImage *getSpecularTexture();
//    QImage *getNormalTexture();
    void setCanvas(Canvas *c);

public slots:
    void randomize();

private:
    QGraphicsScene *diffuseTextureScene;
    QGraphicsScene *specularTextureScene;
    Canvas *canvas;

    void randomizeChildren(QGraphicsItem *parent);
};


#endif // TEXTUREGENERATOR_H
