#include "managers.h"

#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QtMath>

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
    qDebug() << mainMat->shininess();

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
    QVBoxLayout *l = new QVBoxLayout();
    l->addStretch();

    QHBoxLayout *shiNumLO = new QHBoxLayout();
    QLabel *shiNumL = new QLabel(tr("Shininess strenght"));
    QDoubleSpinBox *shiNumDSB = new QDoubleSpinBox();
    shiNumDSB->setRange(0.0, 10.0);
    shiNumDSB->setDecimals(1);
    shiNumDSB->setSingleStep(0.1);
    shiNumDSB->setValue((10.0 - qLn(mainMat->shininess())/qLn(10.0))*10.0/9.9);
    shiNumLO->addWidget(shiNumL);
    shiNumLO->addWidget(shiNumDSB);
    l->addLayout(shiNumLO);

    connect(shiNumDSB, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double s){ mainMat->setShininess(qPow(10, 10.0 - (s*0.99))); qDebug() << mainMat->shininess();});

    l->addStretch();
    return l;
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
//    texGen->getDiffuseTexture()->save("C:/Users/slava/OneDrive/Qt_projects/PlatesEditor/pic/hi.png");
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
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(new QLabel("Just a ModelManager"));

    return l;
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
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(new QLabel("Just a LightManager"));

    return l;
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
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(new QLabel("Just a CameraManager"));

    return l;
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
    savingPath = "C:/Users/slava/OneDrive/Qt_projects/PlatesEditor/pic/";
}

QLayout *RenderManager::getSettingsLayout()
{
    QVBoxLayout *l = new QVBoxLayout();
    l->addStretch();

    QHBoxLayout *picNumLO = new QHBoxLayout();
    QLabel *picNumL = new QLabel(tr("Picture number"));
    QSpinBox *picNumSB = new QSpinBox();
    picNumSB->setRange(1, 5000);
    picNumSB->setValue(picNum);
    picNumLO->addWidget(picNumL);
    picNumLO->addWidget(picNumSB);
    l->addLayout(picNumLO);

    QHBoxLayout *pathLO = new QHBoxLayout();
    QLabel *pathL = new QLabel(tr("Saving path"));
    QLabel *pathDispL = new QLabel(savingPath);
    QPushButton *pathPB = new QPushButton(tr("Choose path"));
    pathLO->addWidget(pathL);
    pathLO->addWidget(pathPB);
    l->addLayout(pathLO);
    l->addWidget(pathDispL);

    connect(picNumSB, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int num){ picNum = num; });
    connect(pathPB, &QPushButton::pressed,
            this, [=](){
              savingPath = QFileDialog::getExistingDirectory(
                              nullptr
                            , "Save path"
                            , ""
                            , QFileDialog::ShowDirsOnly
                            | QFileDialog::DontResolveSymlinks) + "/";
              pathDispL->setText(savingPath);}, Qt::QueuedConnection);

    l->addStretch();

    return l;
}

void RenderManager::randomize()
{

}
