#ifndef OFFSCREENENGINE_H
#define OFFSCREENENGINE_H

#include <Qt3DRender/QRenderTarget>
#include <Qt3DRender/QRenderTargetOutput>
#include <Qt3DRender/QTexture>

#include <QOffscreenSurface>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QRenderTargetSelector>
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QMultiSampleAntiAliasing>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QCameraSelector>

#include <QSharedPointer>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QNode>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QRenderCapture>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DLogic/QLogicAspect>

// Encapsulates a 2D texture that a frame graph can render into.
class TextureRenderTarget : public Qt3DRender::QRenderTarget
{
    Q_OBJECT
public:
    TextureRenderTarget(Qt3DCore::QNode *parent = nullptr,
                        const QSize &size = QSize(500, 500),
                        Qt3DRender::QRenderTargetOutput::AttachmentPoint attatchmentPoint
                                                = Qt3DRender::QRenderTargetOutput::Color0);

    void setSize(const QSize &size);
    QSize getSize() { return size; }
    Qt3DRender::QTexture2DMultisample* getTexture();

private:
    QSize size;
    Qt3DRender::QRenderTargetOutput *output;
    Qt3DRender::QTexture2DMultisample  *texture;
    // To enable depth testing
    Qt3DRender::QRenderTargetOutput *depthTextureOutput;
    Qt3DRender::QTexture2DMultisample  *depthTexture;
};

// The OffscreenSurfaceFrameGraph class is where the magic happens.
// It is responsible for rendering the scene to an "offscreen" surface (ie. a texture),
// rather than directly to a QWindow. This means that the render contents can be
// taken and used within other QWidgets.
class OffscreenSurfaceFrameGraph : public Qt3DRender::QRenderSurfaceSelector
{
    Q_OBJECT
public:
    OffscreenSurfaceFrameGraph(Qt3DCore::QNode* parent = nullptr, Qt3DRender::QCamera *camera = nullptr, const QSize &size = QSize(500, 500));

    void setSize(const QSize &size);
    Qt3DCore::QNode *getRenderTargetSelector();
    Qt3DRender::QClearBuffers *getClearBuffers();

private:
    TextureRenderTarget *textureTarget;
    QOffscreenSurface *offscreenSurface;
    Qt3DRender::QRenderTargetSelector *renderTargetSelector;
    Qt3DRender::QViewport *viewport;
    Qt3DRender::QClearBuffers *clearBuffers;
    Qt3DRender::QCameraSelector *cameraSelector;
    Qt3DRender::QCamera *camera;
};

// The OffscreenEngine brings together various Qt3D classes that are required in order to
// perform basic scene rendering. Of these, the most important for this project is the OffscreenSurfaceFrameGraph.
// Render captures can be requested, and the capture contents used within other widgets (see OffscreenEngineDelegate).
class OffscreenEngine : public QObject
{
    Q_OBJECT
public:
    OffscreenEngine(Qt3DRender::QCamera *camera, const QSize &size);
    ~OffscreenEngine();

    void setSceneRoot(Qt3DCore::QNode *sceneRoot);
    Qt3DRender::QRenderCapture *getRenderCapture();
    void setSize(const QSize &size);

signals:
    void imageRendered(QImage image);

public slots:
    void requestRenderCapture();
    void setClearColor(const QColor &color);

private slots:
    void onImageRendered();

private:
    // We need all of the following in order to render a scene:
    Qt3DCore::QAspectEngine *aspectEngine;              // The aspect engine, which holds the scene and related aspects.
    Qt3DRender::QRenderAspect *renderAspect;            // The render aspect, which deals with rendering the scene.
    Qt3DLogic::QLogicAspect *logicAspect;               // The logic aspect, which runs jobs to do with synchronising frames.
    Qt3DRender::QRenderSettings *renderSettings;        // The render settings, which control the general rendering behaviour.
    Qt3DRender::QRenderCapture *renderCapture;          // The render capture node, which is appended to the frame graph.
    OffscreenSurfaceFrameGraph *offscreenFrameGraph;    // The frame graph, which allows rendering to an offscreen surface.
    Qt3DCore::QNode *sceneRoot;                         // The scene root, which becomes a child of the engine's root entity.

    Qt3DRender::QRenderCaptureReply *reply;
};

#endif // OFFSCREENENGINE_H
