#include "heigthtonormalgraphicseffect.h"

#include <QPainter>
#include <QVector3D>

HeigthToNormalGraphicsEffect::HeigthToNormalGraphicsEffect(QObject *parent)
    : QGraphicsEffect(parent)
{

}

static QVector<int> getBlackNeighComps(const QImage &img, int x, int y);

void HeigthToNormalGraphicsEffect::draw(QPainter *painter)
{
//    const QPixmap pixmap = sourcePixmap();
    QImage height = sourcePixmap().toImage();

    QImage normal = QImage(height.size(), QImage::Format_RGB32);
    QPainter painterD(&normal);

    const float scale = 0.009f;
    for(int i = 0; i < height.height(); i++) {
        for(int j = 0; j < height.width(); j++) {
            QVector3D n;
            auto s = getBlackNeighComps(height, j, i);
            n.setX(scale * -(s[2]-s[0]+2*(s[5]-s[3])+s[8]-s[6]));
            n.setY(scale * -(s[6]-s[0]+2*(s[7]-s[1])+s[8]-s[2]));
            n.setZ(1.0);
            n.normalize();
            n.setX((n.x()+1) * 255.0f / 2.0f);
            n.setY((n.y()+1) * 255.0f / 2.0f);
            n.setZ((n.z() * 127.0f + 128.0f));
            if(n.x() < 0.0f || n.x() > 255.0f)
                qWarning("HERE X");
            if(n.y() < 0.0f || n.y() > 255.0f)
                qWarning("HERE Y");
            if(n.z() < 0.0f || n.z() > 255.0f)
                qWarning("HERE Z");
            normal.setPixelColor(j, i, QColor(n.x(), n.y(), n.z()));
        }
    }
    painter->drawImage(0, 0, height);
}

static QVector<int> getBlackNeighComps(const QImage &img, int x, int y)
{
    static QPoint indexes[] = {
        QPoint(-1, +1), // 0
        QPoint( 0, +1), // 1
        QPoint(+1, +1), // 2
        QPoint(-1,  0), // 3
        QPoint( 0,  0), // 4
        QPoint(+1,  0), // 5
        QPoint(-1, -1), // 6
        QPoint( 0, -1), // 7
        QPoint(+1, -1)  // 8
    };
    QVector<int> res(9, 0);
    for(int i = 0; i < 9; i++) {
        if(indexes[i].x() + x >= img.width()) continue;
        if(indexes[i].y() + y >= img.height()) continue;
        if(indexes[i].x() + x < 0) continue;
        if(indexes[i].y() + y < 0) continue;
        res[i] = img.pixelColor(indexes[i] + QPoint(x, y)).black();
    }
    return res;
}
