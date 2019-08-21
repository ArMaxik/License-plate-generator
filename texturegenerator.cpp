#include "texturegenerator.h"
//#include "texgenhelpfunctions.h"

#include <QRandomGenerator>
#include <QVector3D>
#include <QtMath>

TextureGenerator::TextureGenerator(QObject *parent)
    : QObject (parent)
{
//    font_id = QFontDatabase::addApplicationFont(":/fonts/Driver Gothic.otf");
}


void TextureGenerator::generateTexture()
{
//    QImage *diffuse = new QImage(280, 140, QImage::Format_RGB32);
//    QPainter painterD(diffuse);
//    QImage *height = new QImage(280, 140, QImage::Format_RGB32);
//    QPainter painterH(height);
//    QImage *orangeMask = new QImage(280, 140, QImage::Format_RGB32);
//    QPainter painterOM(orangeMask);

//    QString mainLbl1 = gen_random_main_label1();
//    QString mainLbl2 = gen_random_main_label2();
//    QString smallLbl = gen_random_small_str(6, 12);

//    QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);
//    QFont font(family);

//    // Diffuse
//    QImage back(":/pictures/MyFLORIDA_small.png");
//    painterD.drawImage(QPoint(0, 0), back);

//    drawText(painterD, font, QColor(17, 141, 128), mainLbl1, mainLbl2, smallLbl);

//    // Height
//    painterH.setPen(Qt::black);
//    painterH.setBrush(Qt::black);
//    painterH.drawRect(height->rect());

//    drawText(painterH, font, Qt::white, mainLbl1, mainLbl2, smallLbl);

//    // Orange mask
//    QImage backOM(":/pictures/OrangeMask_small.png");
//    painterOM.drawImage(QPoint(0, 0), backOM);

//    drawText(painterOM, font, Qt::black, mainLbl1, mainLbl2, smallLbl);

//    QImage* height_b = gaussBlur(height, 2);

//    QImage *normal = height2Normal(height_b);
//    painterH.end();
//    delete height;
//    delete height_b;
//    emit diffuseGenerated(diffuse);
//    emit normalGenerated(normal);
//    emit orangeMaskGenerated(orangeMask);
}
