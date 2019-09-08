#include "managers.h"

#include <QLabel>

AbstractManager::AbstractManager(QObject *parent)
    : QObject(parent)
{

}

// ===================== MaterialManager =================================

MaterialManager::MaterialManager(QObject *parent)
    : AbstractManager(parent)
    , mainMat(new MaterialNormalDiffuseSpecular())
{
    name = tr("Material Manager");

//    QObject::connect(texGen, &TextureGenerator::diffuseGenerated,
//                         floridaPlate->getDiffuseTexture(), &ImageTexture::setImage);
//    QObject::connect(texGen, &TextureGenerator::normalGenerated,
//                     floridaPlate->getNormalTexture(), &ImageTexture::setImage);
//    QObject::connect(texGen, &TextureGenerator::orangeMaskGenerated,
//                     orangeMask->getTexture(), &ImageTexture::setImage);
}


TextureGenerator *MaterialManager::getGenerator() const
{
    return texGen;
}

QLayout *MaterialManager::getSettingsLayout()
{
    settingsLayout.layout()->addWidget(new QLabel("Just a MaterialManager"));

    return settingsLayout.layout();
}

void MaterialManager::setTextureGenerator(TextureGenerator *tg)
{
    texGen = tg;
}

void MaterialManager::randomize()
{
    texGen->randomize();
    mainMat->setTextures(texGen->getDiffuseTexture(),
                         texGen->getSpecularTexture(),
                         texGen->getNormalTexture());
    texGen->getDiffuseTexture()->save("C:/Users/slava/OneDrive/Qt_projects/PlatesEditor/pic/hi.png");
}

// ===================== ModelManager =================================

ModelManager::ModelManager(Qt3DCore::QEntity *scene, QObject *parent)
    : AbstractManager(parent)
    , licensePlate(new RenderableEntity(scene))
{
    name = tr("Model Manager");
}

QLayout *ModelManager::getSettingsLayout()
{
    settingsLayout.layout()->addWidget(new QLabel("Just a ModelManager"));

    return settingsLayout.layout();
}

void ModelManager::randomize()
{
        licensePlate->randomize();
}

void ModelManager::setMaterial(Qt3DRender::QMaterial *material)
{
    licensePlate->setMaterial(material);
}

// ===================== LightManager =================================

LightManager::LightManager(Qt3DCore::QEntity *scene, QObject *parent)
    : AbstractManager(parent)
    , mainLight(new PointLight(scene))
    , ambient1(new DirectionalLight(scene))
    , ambient2(new DirectionalLight(scene))
{
    name = tr("Light Manager");

    ambient1->getLight()->setWorldDirection(QVector3D(0.0, -1.0, -1.0));
    ambient2->getLight()->setWorldDirection(QVector3D(0.0, -1.0,  1.0));
}

QLayout *LightManager::getSettingsLayout()
{
    settingsLayout.layout()->addWidget(new QLabel("Just a LightManager"));

    return settingsLayout.layout();
}

void LightManager::randomize()
{
    mainLight->randomize();
    ambient1->getLight()->setIntensity(mainLight->getLight()->intensity() / 4.0f);
    ambient1->getLight()->setColor(mainLight->getLight()->color());
    ambient2->getLight()->setIntensity(mainLight->getLight()->intensity() / 4.0f);
    ambient2->getLight()->setColor(mainLight->getLight()->color());
}

// ===================== CameraManager =================================

CameraManager::CameraManager(Qt3DCore::QEntity *scene, QObject *parent)
    : AbstractManager(parent)
    , camera(new Qt3DRender::QCamera(scene))

{
    name = tr("Camera Manager");

    camera->lens()->setPerspectiveProjection(35.0f, 1.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0.0, 25.0, 0.0));
    camera->setViewCenter(QVector3D(0.0, 5.0, 0.0));
    camera->setUpVector(QVector3D(0.0, 1.0, 0.0));
}

QLayout *CameraManager::getSettingsLayout()
{
    settingsLayout.layout()->addWidget(new QLabel("Just a CameraManager"));

    return settingsLayout.layout();
}

void CameraManager::randomize()
{

}

// ===================== RenderManager =================================


RenderManager::RenderManager(Qt3DCore::QEntity *scene, QObject *parent)
    : AbstractManager(parent)
{
    name = tr("Render Manager");

    picNum = 20;
    savePath = "C:/Users/slava/OneDrive/Qt_projects/PlatesEditor/pic/";
}

QLayout *RenderManager::getSettingsLayout()
{
    settingsLayout.layout()->addWidget(new QLabel("Just a SettingsManager"));

    return settingsLayout.layout();
}

void RenderManager::randomize()
{

}
