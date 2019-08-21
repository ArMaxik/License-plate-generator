#ifndef MANAGERS_H
#define MANAGERS_H

#include "texturegenerator.h"
#include "material.h"

#include <QLayout>

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
    MaterialNormalDiffuseSpecular *floridaPlateMaterial() const;
    MaterialTexture *orangeMaskMaterial() const;
    TextureGenerator *getGenerator() const;
    QLayout *getSettingsLayout() override;

public slots:
    void randomize() override;

private:
    TextureGenerator *texGen;
    MaterialNormalDiffuseSpecular *floridaPlate;
    MaterialTexture *orangeMask;
};

#endif // MANAGERS_H
