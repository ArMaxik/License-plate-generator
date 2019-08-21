#ifndef MATERIAL_H
#define MATERIAL_H

#include <Qt3DExtras/QNormalDiffuseSpecularMapMaterial>
#include <Qt3DExtras/QTextureMaterial>

#include "imagetexture.h"

class MaterialNormalDiffuseSpecular : public Qt3DExtras::QNormalDiffuseSpecularMapMaterial
{
    Q_OBJECT
public:
    MaterialNormalDiffuseSpecular(Qt3DCore::QNode *parent = nullptr);

    ImageTexture *getDiffuseTexture() const;
    ImageTexture *getSpecularTexture() const;
    ImageTexture *getNormalTexture() const;

public slots:
    void setTextures(QImage* diffuse, QImage* normal);  // Уже не нужен

private:
    ImageTexture *m_diffuseTextureImage;
    ImageTexture *m_normalTextureImage;
    ImageTexture *m_specularTextureImage;
};

class MaterialTexture : public Qt3DExtras::QTextureMaterial
{
    Q_OBJECT
public:
    MaterialTexture(Qt3DCore::QNode *parent = nullptr);

    ImageTexture *getTexture() const;

private:
    ImageTexture *m_texture;
};

#endif // MATERIAL_H
