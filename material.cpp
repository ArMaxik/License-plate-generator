#include "material.h"

MaterialNormalDiffuseSpecular::MaterialNormalDiffuseSpecular(Qt3DCore::QNode *parent)
    : Qt3DExtras::QNormalDiffuseSpecularMapMaterial(parent)
    , m_diffuseTextureImage(new ImageTexture())
    , m_normalTextureImage(new ImageTexture())
    , m_specularTextureImage(new ImageTexture())
{
    diffuse()->addTextureImage(m_diffuseTextureImage);
    normal()->addTextureImage(m_normalTextureImage);
    specular()->addTextureImage(m_specularTextureImage);
}

ImageTexture *MaterialNormalDiffuseSpecular::getDiffuseTexture() const
{
    return m_diffuseTextureImage;
}

ImageTexture *MaterialNormalDiffuseSpecular::getSpecularTexture() const
{
    return m_specularTextureImage;
}

ImageTexture *MaterialNormalDiffuseSpecular::getNormalTexture() const
{
    return m_normalTextureImage;
}

void MaterialNormalDiffuseSpecular::setTextures(QImage *diffuse, QImage *normal)
{
    m_diffuseTextureImage->setImage(diffuse);
    m_normalTextureImage->setImage(normal);
//    m_specularTextureImage->setImage(specular);

    m_diffuseTextureImage->update();
    m_normalTextureImage->update();
    m_specularTextureImage->update();
}

MaterialTexture::MaterialTexture(Qt3DCore::QNode *parent)
    : Qt3DExtras::QTextureMaterial (parent)
    , m_texture(new ImageTexture())
{
    texture()->addTextureImage(m_texture);
}

ImageTexture *MaterialTexture::getTexture() const
{
    return m_texture;
}
