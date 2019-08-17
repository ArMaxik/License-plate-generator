#include "textureeditorwidget.h"

#include <QVBoxLayout>
#include <QTabWidget>

TextureEditorWidget::TextureEditorWidget(QWidget *parent)
    : CustomCentralWidget(parent)
    , treeModel(new ItemsTreeModel())
    , controllScene(new QGraphicsScene(this))
    , controllView(new ControllViewPanel(controllScene))
    , diffuseScene(new QGraphicsScene(this))
    , diffuseView(new ControllViewPanel(diffuseScene))
    , specularScene(new QGraphicsScene(this))
    , specularView(new ControllViewPanel(specularScene))
    , normalScene(new QGraphicsScene(this))
    , normalView(new ControllViewPanel(normalScene))
{
    connect(controllScene, &QGraphicsScene::selectionChanged,
            this, &TextureEditorWidget::onItemSelected);

    Canvas *canvas = new Canvas(QSize(200, 200));
    treeModel->setCanvas(canvas);

    setUpLayout();
}

QGraphicsScene *TextureEditorWidget::getControllScene() const
{
    return controllScene;
}

ItemsTreeModel *TextureEditorWidget::getItemsTreeModel() const
{
    return treeModel;
}

void TextureEditorWidget::addItem()
{
    BasicItem *item = new BasicItem();
    treeModel->addItem(item);

    controllScene->addItem(item);
    diffuseScene->addItem(item->getDiffuseChanel());
    specularScene->addItem(item->getSpecularChanel());
}

void TextureEditorWidget::setUpLayout()
{
    QVBoxLayout *mainL = new QVBoxLayout();
    setLayout(mainL);

    QTabWidget *tw = new QTabWidget(this);
    mainL->addWidget(tw);

    QWidget *editTab = new QWidget();
    QVBoxLayout *etL = new QVBoxLayout();
    etL->addWidget(controllView, 0, Qt::AlignCenter);
    editTab->setLayout(etL);
    tw->addTab(editTab, tr("Edit"));

    QWidget *diffuseTab = new QWidget();
    QVBoxLayout *diL = new QVBoxLayout();
    diL->addWidget(diffuseView, 0, Qt::AlignCenter);
    diffuseTab->setLayout(diL);
    tw->addTab(diffuseTab, tr("Diffuse chanel"));

    QWidget *specularTab = new QWidget();
    QVBoxLayout *spL = new QVBoxLayout();
    spL->addWidget(specularView, 0, Qt::AlignCenter);
    specularTab->setLayout(spL);
    tw->addTab(specularTab, tr("Specular chanel"));

}

void TextureEditorWidget::onItemSelected()
{
    if(controllScene->selectedItems().empty()) {
        emit itemSelected(nullptr);
    } else {
        QGraphicsItem *hmm = controllScene->selectedItems().takeLast();
        BasicItem *item = dynamic_cast<BasicItem*>(hmm);
        emit itemSelected(item);
    }
}
