#include "textureeditorwidget.h"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QFileDialog>

TextureEditorWidget::TextureEditorWidget(QWidget *parent)
    : QWidget(parent)
    , treeModel(new ItemsTreeModel())
    , controllView(new CanvasViewWidget())
    , diffuseView(new ImageViewWiget())
    , specularView(new ImageViewWiget())
    , normalView(new ImageViewWiget())
    , texGen(new TextureGenerator())
{
    connect(controllView->getScene(), &QGraphicsScene::selectionChanged,
            this, &TextureEditorWidget::onItemSelected);

    Canvas *canvas = new Canvas(QSize(600, 400));
    canvas->setUpChanels();
    controllView->addItem(canvas);

    texGen->setCanvas(canvas);
    treeModel->setCanvas(canvas);
    controllView->setCanvas(canvas);

    connect(canvas, &Canvas::changed,
            this, &TextureEditorWidget::updateImageViewers);

    updateImageViewers();

    setUpLayout();
}

ItemsTreeModel *TextureEditorWidget::getItemsTreeModel() const
{
    return treeModel;
}

void TextureEditorWidget::addItem()
{
    BasicItem *item = new BasicItem();
    item->setUpChanels();
    treeModel->addItem(item);

    connect(item, &BasicItem::changed,
            this, &TextureEditorWidget::updateImageViewers);
}

void TextureEditorWidget::addItem(BasicItem *item)
{
    treeModel->addItem(item);

    connect(item, &BasicItem::changed,
            this, &TextureEditorWidget::updateImageViewers);
}

void TextureEditorWidget::randomize()
{
    texGen->randomize();
    updateImageViewers();
}

void TextureEditorWidget::clear()
{
    treeModel->clear();
}

void TextureEditorWidget::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Template"), "",
            tr("LiscensePlateXML (*.lp.xml);;All Files (*)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    treeModel->save(&file);
}

void TextureEditorWidget::setUpLayout()
{
    QVBoxLayout *mainL = new QVBoxLayout();
    setLayout(mainL);

    QTabWidget *tw = new QTabWidget(this);
    mainL->addWidget(tw);

    tw->addTab(controllView, tr("Edit"));
    tw->addTab(diffuseView, tr("Diffuse chanel"));
    tw->addTab(specularView, tr("Specular chanel"));
    tw->addTab(normalView, tr("Normal chanel"));
}

void TextureEditorWidget::onItemSelected()
{
    if(controllView->getScene()->selectedItems().empty()) {
        emit itemSelected(nullptr);
    } else {
        QGraphicsItem *hmm = controllView->getScene()->selectedItems().takeLast();
        BasicItem *item = dynamic_cast<BasicItem*>(hmm);
        emit itemSelected(item);
    }
}

void TextureEditorWidget::updateImageViewers()
{
    diffuseView->setImage(texGen->getDiffuseTexture());
    specularView->setImage(texGen->getSpecularTexture());
    normalView->setImage(texGen->getNormalTexture());
}
