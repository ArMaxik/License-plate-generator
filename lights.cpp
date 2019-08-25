#include "lights.h"

#include <QRandomGenerator>

PointLight::PointLight(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_light(new Qt3DRender::QPointLight())
    , m_transform(new Qt3DCore::QTransform())
{
    m_transform->setTranslation(QVector3D(0.0, 25.0, 10.0));

    addComponent(m_light);
    addComponent(m_transform);
}

Qt3DRender::QPointLight *PointLight::getLight() const
{
    return m_light;
}

Qt3DCore::QTransform *PointLight::getTransform() const
{
    return m_transform;
}

void PointLight::randomize()
{
    m_light->setColor(QColor(QRandomGenerator::global()->bounded(200, 256),
                             QRandomGenerator::global()->bounded(200, 256),
                             QRandomGenerator::global()->bounded(200, 256)));

    m_light->setIntensity(QRandomGenerator::global()->bounded(5, 12)/10.0f);

    m_transform->setTranslation(QVector3D(QRandomGenerator::global()->bounded(-100, 100) / 10.0f,
                                          QRandomGenerator::global()->bounded(-100, 100) / 10.0f + 20.0f,
                                          QRandomGenerator::global()->bounded(-100, 100) / 10.0f));
}

DirectionalLight::DirectionalLight(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_light(new Qt3DRender::QDirectionalLight)
{
    m_light->setIntensity(1.0);
    m_light->setColor(Qt::white);
    m_light->setWorldDirection(QVector3D(0.0, -1.0, 1.0));

    addComponent(m_light);
}

Qt3DRender::QDirectionalLight *DirectionalLight::getLight()
{
    return m_light;
}

void DirectionalLight::randomize()
{
}
