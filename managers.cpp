#include "managers.h"

#include <QLabel>

AbstractManager::AbstractManager(QObject *parent)
    : QObject(parent)
{

}

MaterialManager::MaterialManager(QObject *parent)
    : AbstractManager(parent)
    , texGen(new TextureGenerator(this))
    , floridaPlate(new MaterialNormalDiffuseSpecular())
    , orangeMask(new MaterialTexture())
{
    name = tr("Material Manager");

    QObject::connect(texGen, &TextureGenerator::diffuseGenerated,
                         floridaPlate->getDiffuseTexture(), &ImageTexture::setImage);
    QObject::connect(texGen, &TextureGenerator::normalGenerated,
                     floridaPlate->getNormalTexture(), &ImageTexture::setImage);
    QObject::connect(texGen, &TextureGenerator::orangeMaskGenerated,
                     orangeMask->getTexture(), &ImageTexture::setImage);
}

MaterialNormalDiffuseSpecular *MaterialManager::floridaPlateMaterial() const
{
    return floridaPlate;
}

MaterialTexture *MaterialManager::orangeMaskMaterial() const
{
    return orangeMask;
}

TextureGenerator *MaterialManager::getGenerator() const
{
    return texGen;
}

QLayout *MaterialManager::getSettingsLayout()
{
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(new QLabel("Hi mark"));

    return l;
}

void MaterialManager::randomize()
{
    texGen->generateTexture();
}
