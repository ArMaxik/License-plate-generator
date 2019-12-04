#ifndef MANAGERS_H
#define MANAGERS_H

#include "texturegenerator.h"
#include "material.h"
#include "renderableentity.h"
#include "lights.h"

#include <QLayout>
#include <Qt3DRender/QCamera>

class AbstractManager : public QObject
{
    Q_OBJECT
public:
    AbstractManager(QObject *parent);

    virtual QLayout *getSettingsLayout() = 0;
    const QString &getName() const { return name; }

public slots:
    virtual void randomize() = 0;

protected:
    QString name;
};

class MaterialManager : public AbstractManager
{
    Q_OBJECT
public:
    MaterialManager(QObject *parent = nullptr);
    TextureGenerator *getGenerator() const;

    QLayout *getSettingsLayout() override;

    void setTextureGenerator(TextureGenerator *tg);

    MaterialNormalDiffuseSpecular *getMainMaterial() const { return mainMat; }
    const QVector<MaterialTexture*> *getMasksMaterials() const { return &masksMat; }

public slots:
    void randomize() override;

private:
    TextureGenerator *texGen;
    MaterialNormalDiffuseSpecular *mainMat;
    QVector<MaterialTexture*> masksMat;

};

class ModelManager : public AbstractManager
{
    Q_OBJECT
public:
    ModelManager(Qt3DCore::QEntity *scene = nullptr, QObject *parent = nullptr);

    QLayout *getSettingsLayout() override;

public slots:
    void randomize() override;
    void setMaterial(Qt3DRender::QMaterial *material);

private:
    RenderableEntity *licensePlate;
};

class LightManager : public AbstractManager
{
    Q_OBJECT
public:
    LightManager(Qt3DCore::QEntity *scene = nullptr, QObject *parent = nullptr);

    QLayout *getSettingsLayout() override;

public slots:
    void randomize() override;

private:
    PointLight *mainLight;
    DirectionalLight *ambient1;
    DirectionalLight *ambient2;
};

class CameraManager : public AbstractManager
{
    Q_OBJECT
public:
    CameraManager(Qt3DCore::QEntity *scene = nullptr, QObject *parent = nullptr);

    QLayout *getSettingsLayout() override;

    Qt3DRender::QCamera *getCamera() const { return camera; }

public slots:
    void randomize() override;

private:
    Qt3DRender::QCamera *camera;
};

class RenderManager : public AbstractManager
{
    Q_OBJECT
public:
    RenderManager(Qt3DCore::QEntity *scene = nullptr, QObject *parent = nullptr);

    QLayout *getSettingsLayout() override;

    int getPictureCount() const { return picNum; }
    QSize getImageSize() const { return QSize(picW, picH); }
    const QString &getSavePath() const { return savingPath; }

public slots:
    void randomize() override;

private:
    int picW;
    int picH;
    int picNum;
    QString savingPath;

};

#endif // MANAGERS_H
