#ifndef TEXTUREGENERATOR_H
#define TEXTUREGENERATOR_H

#include <QImage>
#include <QPen>
#include <QBrush>
#include <QPainter>

#include <QFontDatabase>

class TextureGenerator : public QObject
{
    Q_OBJECT
public:
    TextureGenerator(QObject *parent);
public slots:
    void generateTexture();
signals:
    void diffuseGenerated(QImage *diffuse);
    void normalGenerated(QImage *normal);
    void orangeMaskGenerated(QImage *orangeMask);
    void heightGenerated(QImage *height);
private:
    int font_id;
};


#endif // TEXTUREGENERATOR_H
