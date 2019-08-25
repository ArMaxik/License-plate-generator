#include "renderableentity.h"

#include <QRandomGenerator>

RenderableEntity::RenderableEntity(Qt3DCore::QNode *parent, Qt3DRender::QMaterial *material)
    : Qt3DCore::QEntity(parent)
    , m_mesh(new Qt3DExtras::QPlaneMesh)
    , m_transform(new Qt3DCore::QTransform())
    , m_material(material)
{
    m_mesh->setHeight(5.0f);
    m_mesh->setWidth(10.0f);

    m_transform->setScale(1.0f);
    m_transform->setTranslation(QVector3D(0.0f, 5.0f, 0.0f));  // Текущий RenderGraph плохо работает, если меш находится в нулевой точке
    m_transform->setRotationX(QRandomGenerator::global()->bounded(-200, 200)/10);
    m_transform->setRotationY(QRandomGenerator::global()->bounded(-200, 200)/10);
    m_transform->setRotationZ(QRandomGenerator::global()->bounded(-200, 200)/10);

    addComponent(m_mesh);
    addComponent(m_transform);
    if(m_material != nullptr) {
        addComponent(m_material);
    }
}

void RenderableEntity::randomize()
{
    m_transform->setRotationX(QRandomGenerator::global()->bounded(-200, 200)/10);
    m_transform->setRotationY(QRandomGenerator::global()->bounded(-200, 200)/10);
    m_transform->setRotationZ(QRandomGenerator::global()->bounded(-200, 200)/10);
}

void RenderableEntity::setMaterial(Qt3DRender::QMaterial *material)
{
    if(m_material != nullptr) {
        removeComponent(m_material);
    }
    m_material = material;  // Memmory leak?
    addComponent(m_material);
}
