#ifndef LIGHTS_H
#define LIGHTS_H


#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <QPointLight>
#include <QDirectionalLight>


class PointLight : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    PointLight(Qt3DCore::QNode *parent = nullptr);
    Qt3DRender::QPointLight *getLight() const;
    Qt3DCore::QTransform *getTransform() const;

public slots:
    void randomize();

private:
    Qt3DRender::QPointLight *m_light;
    Qt3DCore::QTransform *m_transform;
};


class DirectionalLight : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    DirectionalLight(Qt3DCore::QNode *parent = nullptr);
    Qt3DRender::QDirectionalLight *getLight();

public slots:
    void randomize();

private:
    Qt3DCore::QTransform *m_transform;
    Qt3DRender::QDirectionalLight *m_light;
};
#endif // LIGHTS_H
