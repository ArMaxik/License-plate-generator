#ifndef HEIGTHTONORMALGRAPHICSEFFECT_H
#define HEIGTHTONORMALGRAPHICSEFFECT_H

#include <QGraphicsEffect>

class HeigthToNormalGraphicsEffect : public QGraphicsEffect
{
public:
    HeigthToNormalGraphicsEffect(QObject *parent = nullptr);

    void draw(QPainter *painter) override;

};

#endif // HEIGTHTONORMALGRAPHICSEFFECT_H
