#include "modeleditorwidget.h"
#include "managers.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


RenderStep::RenderStep(Qt3DRender::QMaterial *material, RenderStep *next)
    : nextStep(next)
    , mat(material)
{

}

FirstRenderStep::FirstRenderStep(Qt3DRender::QMaterial *material, RenderStep *next)
    : RenderStep(material, next)
    , renderNum(0)
{

}

SceneController::SceneController(QObject *parent)
    : QObject(parent)
    , scene(new Qt3DCore::QEntity())
    , materialsM(new MaterialManager(this))
    , lightsM(new LightManager(scene, this))
    , cameraM(new CameraManager(scene, this))
    , modelM(new ModelManager(scene, this))
    , renderM(new RenderManager(scene, this))
    , renderEngine(new OffscreenEngine(cameraM->getCamera(), QSize(400, 400)))
    , firstRenderStep(nullptr)
    , currentRenderStep(nullptr)
{
    renderEngine->setSceneRoot(scene);

    connect(renderEngine, &OffscreenEngine::imageRendered,
            this, &SceneController::acceptRender);
}

void SceneController::setUpRender()
{
    delete firstRenderStep;
    delete currentRenderStep;

    firstRenderStep = new FirstRenderStep(nullptr, nullptr);
    firstRenderStep->renderNum = renderM->getPictureCount();
    currentRenderStep = firstRenderStep;

    doFirstRenderStep();
}

void SceneController::doFirstRenderStep()
{
    if(firstRenderStep->renderNum == 0) {
        return;
    }
    materialsM->randomize();
    lightsM->randomize();
    cameraM->randomize();
    modelM->randomize();

    modelM->setMaterial(materialsM->getMainMaterial());

    renderEngine->setClearColor(QColor(100, 100, 100, 255));
    firstRenderStep->renderNum--;

    renderEngine->requestRenderCapture();
}

void SceneController::doNextRenderStep()
{
    if(currentRenderStep->nextStep == nullptr) {
        doFirstRenderStep();
    } else {
        currentRenderStep = currentRenderStep->nextStep;
        renderEngine->setClearColor(Qt::black);
        renderEngine->requestRenderCapture();
    }
}

void SceneController::acceptRender(QImage img)
{
    img.save(renderM->getSavePath()
              + QString::number(firstRenderStep->renderNum)
              + QString(".png"));
    doNextRenderStep();
}

ModelEditorWidget::ModelEditorWidget(TextureGenerator *tg, QWidget *parent)
    : QWidget(parent)
    , treeModel(new SceneTreeModel())
    , sceneController(new SceneController(this))
{
    QVBoxLayout *mainL = new QVBoxLayout();
    setLayout(mainL);

    QPushButton *renderButton = new QPushButton(tr("Render"));
    mainL->addWidget(renderButton);

    connect(renderButton, &QPushButton::clicked,
            sceneController, &SceneController::setUpRender);

    sceneController->getMaterialsManager()->setTextureGenerator(tg);
    treeModel->addItem(sceneController->getMaterialsManager());
    treeModel->addItem(sceneController->getModelManager());
    treeModel->addItem(sceneController->getCameraManager());
    treeModel->addItem(sceneController->getLightsManager());
    treeModel->addItem(sceneController->getRenderManager());

}
