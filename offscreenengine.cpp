#include "offscreenengine.h"


TextureRenderTarget::TextureRenderTarget(Qt3DCore::QNode *parent,
                                         const QSize &size,
                                         Qt3DRender::QRenderTargetOutput::AttachmentPoint attatchmentPoint) :
    Qt3DRender::QRenderTarget(parent),
    size(size)
{
    // The lifetime of the objects created here is managed
    // automatically, as they become children of this object.

    // Create a render target output for rendering colour.
    output = new Qt3DRender::QRenderTargetOutput(this);
    output->setAttachmentPoint(attatchmentPoint);

    // Create a texture to render into.
    texture = new Qt3DRender::QTexture2DMultisample(output);
    texture->setSize(size.width(), size.height());
    texture->setFormat(Qt3DRender::QAbstractTexture::RGB8_UNorm);
    texture->setMinificationFilter(Qt3DRender::QAbstractTexture::Linear);
    texture->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);

    // Hook the texture up to our output, and the output up to this object.
    output->setTexture(texture);
    addOutput(output);

    depthTextureOutput = new Qt3DRender::QRenderTargetOutput(this);
    depthTextureOutput->setAttachmentPoint(Qt3DRender::QRenderTargetOutput::Depth);
    depthTexture = new Qt3DRender::QTexture2DMultisample (depthTextureOutput);
    depthTexture->setSize(size.width(), size.height());
    depthTexture->setFormat(Qt3DRender::QAbstractTexture::DepthFormat);
    depthTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::Linear);
    depthTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    depthTexture->setComparisonFunction(Qt3DRender::QAbstractTexture::CompareLessEqual);
    depthTexture->setComparisonMode(Qt3DRender::QAbstractTexture::CompareRefToTexture);
    // Hook up the depth texture
    depthTextureOutput->setTexture(depthTexture);
    addOutput(depthTextureOutput);
}

Qt3DRender::QTexture2DMultisample* TextureRenderTarget::getTexture()
{
    return texture;
}

void TextureRenderTarget::setSize(const QSize &size)
{
    this->size = size;
    texture->setSize(size.width(), size.height());
}

/*
 * An overview of the frame graph used by this class:
 *
 *                 (0)|   QRenderStateSet    |
 *                               V
 *                 (1)| RenderTargetSelector |
 *                               |
 *                      +=================+
 *                      V                 V
 *              (2)| ClearBuffers |  (3)| Viewport |
 *                                           V
 *                                  (4)| CameraSelector |
 *
 * To summarise the Qt3D documentation, the frame graph is processed in depth-first order.
 * Each node sets some state. Therefore you can read the graph as saying:
 * "Given the action for this node, perform the actions of each of its children in order."
 *
 * 1. The render target selector sets the current render target (an OpenGL texture buffer).
 * 2. The specified buffers (here, colour and depth) are cleared.
 * 3. The viewport area to render into is set up.
 * 4. The appropriate camera is chosen.
 *
 * Further children can be added to the render target selector in order to perform subsequent
 * actions. In this example project, the offscreen engine adds a render capture node after
 * the existing children in the frame graph.
 *
 * An example extension of the framegraph can be the one that draws an image in the background:
 *
 *      https://github.com/Sonnentierchen/Qt3D-BackgroundImage
 */

OffscreenSurfaceFrameGraph::OffscreenSurfaceFrameGraph(Qt3DCore::QNode* parent, Qt3DRender::QCamera *camera, const QSize &size) :
    Qt3DRender::QRenderSurfaceSelector(parent),
    camera(camera)
{
    // Firstly, create the offscreen surface. This will take the place
    // of a QWindow, allowing us to render our scene without one.
    offscreenSurface = new QOffscreenSurface();
    offscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
    offscreenSurface->create();

    // Hook it up to the frame graph.
    setSurface(offscreenSurface);
    setExternalRenderTargetSize(size);

    // For antialiasing
    Qt3DRender::QRenderStateSet *renderStateSet = new Qt3DRender::QRenderStateSet(this);

    Qt3DRender::QMultiSampleAntiAliasing *msaa = new Qt3DRender::QMultiSampleAntiAliasing;
    renderStateSet->addRenderState(msaa);
    Qt3DRender::QDepthTest *depthTest = new Qt3DRender::QDepthTest;
    depthTest->setDepthFunction(Qt3DRender::QDepthTest::LessOrEqual);
    renderStateSet->addRenderState(depthTest);

    // Create a texture to render into. This acts as the buffer that
    // holds the rendered image.
    renderTargetSelector = new Qt3DRender::QRenderTargetSelector(renderStateSet);
    textureTarget = new TextureRenderTarget(renderTargetSelector, size);
    renderTargetSelector->setTarget(textureTarget);

    // Create a node used for clearing the required buffers.
    clearBuffers = new Qt3DRender::QClearBuffers(renderTargetSelector);
    clearBuffers->setClearColor(QColor(100, 100, 100, 255));
    clearBuffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);

    // Create a viewport node. The viewport here just covers the entire render area.
    viewport = new Qt3DRender::QViewport(renderTargetSelector);
    viewport->setNormalizedRect(QRectF(0.0, 0.0, 1.0, 1.0));

    // Create a camera selector node, and tell it to use the camera we've ben given.
    cameraSelector = new Qt3DRender::QCameraSelector(viewport);
    cameraSelector->setCamera(camera);
}

void OffscreenSurfaceFrameGraph::setSize(const QSize &size)
{
    textureTarget->setSize(size);
    setExternalRenderTargetSize(size);
}

Qt3DCore::QNode *OffscreenSurfaceFrameGraph::getRenderTargetSelector()
{
    return renderTargetSelector;
}

Qt3DRender::QClearBuffers *OffscreenSurfaceFrameGraph::getClearBuffers()
{
    return clearBuffers;
}

OffscreenEngine::OffscreenEngine(Qt3DRender::QCamera *camera, const QSize &size)
{
    sceneRoot = nullptr;
    reply = nullptr;

    // Set up the engine and the aspects that we want to use.
    aspectEngine = new Qt3DCore::QAspectEngine();
    renderAspect = new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Threaded); // Only threaded mode seems to work right now.
    logicAspect = new Qt3DLogic::QLogicAspect();

    aspectEngine->registerAspect(renderAspect);
    aspectEngine->registerAspect(logicAspect);

    // Create the root entity of the engine.
    // This is not the same as the 3D scene root: the QRenderSettings
    // component must be held by the root of the QEntity tree,
    // so it is added to this one. The 3D scene is added as a subtree later,
    // in setSceneRoot().
    Qt3DCore::QEntityPtr root(new Qt3DCore::QEntity());
    renderSettings = new Qt3DRender::QRenderSettings(root.data());
    root->addComponent(renderSettings);

    // Create the offscreen frame graph, which will manage all of the resources required
    // for rendering without a QWindow.
    offscreenFrameGraph = new OffscreenSurfaceFrameGraph(renderSettings, camera, size);

    // Set this frame graph to be in use.
    renderSettings->setActiveFrameGraph(offscreenFrameGraph);

    // Add a render capture node to the frame graph.
    // This is set as the next child of the render target selector node,
    // so that the capture will be taken from the specified render target
    // once all other rendering operations have taken place.
    renderCapture = new Qt3DRender::QRenderCapture(offscreenFrameGraph->getRenderTargetSelector());

    // Set the root entity of the engine. This causes the engine to begin running.
    aspectEngine->setRootEntity(root);
}

OffscreenEngine::~OffscreenEngine()
{
    delete reply;

    // Setting a null root entity shuts down the engine.
    aspectEngine->setRootEntity(Qt3DCore::QEntityPtr());

    // Not sure if the following is strictly required, as it may
    // happen automatically when the engine is destroyed.
    aspectEngine->unregisterAspect(logicAspect);
    aspectEngine->unregisterAspect(renderAspect);
    delete logicAspect;
    delete renderAspect;

    delete aspectEngine;
}

void OffscreenEngine::setSceneRoot(Qt3DCore::QNode *sceneRoot)
{
    // Make sure any existing scene root is unparented.
    if ( this->sceneRoot )
    {
        this->sceneRoot->setParent(static_cast<Qt3DCore::QNode*>(nullptr));
    }

    // Parent the incoming scene root to our current root entity.
    this->sceneRoot = sceneRoot;
    this->sceneRoot->setParent(aspectEngine->rootEntity().data());
}

Qt3DRender::QRenderCapture* OffscreenEngine::getRenderCapture()
{
    return renderCapture;
}

void OffscreenEngine::setSize(const QSize &size)
{
    offscreenFrameGraph->setSize(size);
}

void OffscreenEngine::requestRenderCapture()
{
    delete reply;
    reply = renderCapture->requestCapture();
    connect(reply, SIGNAL(completed()), this, SLOT(onImageRendered()));
}

void OffscreenEngine::setClearColor(const QColor &color)
{
    offscreenFrameGraph->getClearBuffers()->setClearColor(color);
}

void OffscreenEngine::onImageRendered()
{
    emit imageRendered(reply->image());
}
