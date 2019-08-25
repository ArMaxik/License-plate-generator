#ifndef MODELEDITORWIDGET_H
#define MODELEDITORWIDGET_H

#include "treemodel.h"
#include "managers.h"
#include "offscreenengine.h"

#include <QWidget>

class RenderStep
{
public:
    RenderStep(Qt3DRender::QMaterial *material, RenderStep *next);

    RenderStep *nextStep;
    Qt3DRender::QMaterial *mat;
private:
};

class FirstRenderStep : public RenderStep
{
public:
    FirstRenderStep(Qt3DRender::QMaterial *material, RenderStep *next);
    int renderNum;
};

class SceneController : public QObject
{
    Q_OBJECT
public:
    SceneController(QObject *parent = nullptr);

    MaterialManager *getMaterialsManager() { return materialsM; }
    LightManager *getLightsManager()       { return lightsM;    }
    CameraManager *getCameraManager()      { return cameraM;    }
    ModelManager *getModelManager()        { return modelM;     }
    RenderManager *getRenderManager()      { return renderM;    }

public slots:
    void setUpRender();

private:
    Qt3DCore::QEntity *scene;

    MaterialManager *materialsM;
    LightManager *lightsM;
    CameraManager *cameraM;
    ModelManager *modelM;
    RenderManager *renderM;

    OffscreenEngine *renderEngine;

    FirstRenderStep *firstRenderStep;
    RenderStep *currentRenderStep;

    void doFirstRenderStep();
    void doNextRenderStep();

private slots:
    void acceptRender(QImage img);
};

class ModelEditorWidget : public QWidget
{
public:
    ModelEditorWidget(TextureGenerator *tg, QWidget *parent = nullptr);

    SceneTreeModel *getSceneTreeModel() const { return treeModel; }

private:
    SceneTreeModel *treeModel;
    SceneController *sceneController;
};

#endif // MODELEDITORWIDGET_H
