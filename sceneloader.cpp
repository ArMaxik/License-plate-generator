#include "sceneloader.h"

#include <QFileDialog>
#include <QMap>

static QMap<QString, QDomNode> *getNodeProperties(const QDomNode &nodeXml)
{
    QMap<QString, QDomNode> *map = new QMap<QString, QDomNode>();  // Smart pointers!!!
    auto properies = nodeXml.childNodes();

    for(int i = 0; i < properies.count(); i++) {
        map->insert(properies.item(i).attributes().namedItem("label").nodeValue(),
                   properies.item(i));
    }
    return map;
}

SceneLoader::SceneLoader(TextureEditorWidget *textureEditor, QObject *parent)
    : QObject(parent)
    , texEditor(textureEditor)
{

}

void SceneLoader::loadScene()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
            tr("Load Scene"), "",
            tr("LiscensePlateXML (*.lp.xml);;All Files (*)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QDomDocument doc("scene");

    QString error;
    int line, column;

    if (!doc.setContent(&file, false, &error, &line, &column)) {
        file.close();
        qDebug() << error << " " << line << " " << column;
        return;
    }
    file.close();

    QDomElement scene = doc.documentElement();
    QDomNodeList basicItems = scene.elementsByTagName("BasicItem");

    texEditor->clear();
    for(int i = 0; i < basicItems.count(); i++) {
        texEditor->addItem(createItem(basicItems.item(i)));
    }
}

BasicItem *SceneLoader::createItem(const QDomNode &BasicItemXml)
{
    BasicItem *bi = new BasicItem();

    bi->setUpChanels();
    bi->setName(BasicItemXml.attributes().namedItem("name").nodeValue());
    bi->setX(BasicItemXml.firstChildElement("X").text().toInt());
    bi->setY(BasicItemXml.firstChildElement("Y").text().toInt());
    bi->setZValue(BasicItemXml.firstChildElement("Z").text().toInt());

    BasicChanel::Nodes *currentNode = new BasicChanel::Nodes;
    QDomNode diffChanel = BasicItemXml.firstChildElement("DiffuseChanel");
    bi->getDiffuseChanel()->setNewNode(createNode(diffChanel.firstChildElement("BasicNode"), bi->getBoundRect(), currentNode));
    bi->getDiffuseChanel()->setCurrentNode(*currentNode);

    QDomNode specChanel = BasicItemXml.firstChildElement("SpecularChanel");
    bi->getDiffuseChanel()->setNewNode(createNode(diffChanel.firstChildElement("BasicNode"), bi->getBoundRect(), currentNode));
    bi->getDiffuseChanel()->setCurrentNode(*currentNode);

    QDomNode normChanel = BasicItemXml.firstChildElement("NormalChanel");
    bi->getDiffuseChanel()->setNewNode(createNode(diffChanel.firstChildElement("BasicNode"), bi->getBoundRect(), currentNode));
    bi->getDiffuseChanel()->setCurrentNode(*currentNode);
    delete currentNode;

    return bi;
}

BasicNode *SceneLoader::createNode(const QDomNode &nodeXml, BoundRect *br, BasicChanel::Nodes *nodeType)
{
    BasicNode *node = nullptr;
    auto properties = getNodeProperties(nodeXml);

    *nodeType =  BasicChanel::Nodes(nodeXml.attributes().namedItem("type").nodeValue().toInt());

    if(*nodeType == BasicChanel::Nodes::ImageN) {
        ImageNode *iNode = new ImageNode(br);
        iNode->setWidthProperie(createNumberPropertie(properties->value("Image Width")));
        iNode->setHeightPropertie(createNumberPropertie(properties->value("Image Height")));
        iNode->setImageProperie(createImagePropertie(properties->value("Image")));
        node = iNode;
    } else if(*nodeType == BasicChanel::Nodes::TextN) {
        TextNode *tNode = new TextNode(br);
        tNode->setStringProperie(createStringPropertie(properties->value("Text")));
        tNode->setFontSizeProperie(createNumberPropertie(properties->value("Text size")));
        tNode->setColortPropertie(createColorPropertie(properties->value("Text Color")));
        node = tNode;
    }

    return node;
}

NumberPropertie *SceneLoader::createNumberPropertie(const QDomNode &propertyXml)
{
    QString label = propertyXml.attributes().namedItem("label").nodeValue();
    int value = propertyXml.firstChildElement("value").text().toInt();
    int min = propertyXml.firstChildElement("min").text().toInt();
    int max = propertyXml.firstChildElement("max").text().toInt();

    NumberPropertie *np = new NumberPropertie(label, value, min, max);
    return np;
}

ImagePropertie *SceneLoader::createImagePropertie(const QDomNode &propertyXml)
{
    QString label = propertyXml.attributes().namedItem("label").nodeValue();
    QString imgPath = propertyXml.firstChildElement("imgPath").text();

    ImagePropertie *ip = new ImagePropertie(label);
    ip->setImagePath(imgPath);
    return ip;
}

StringPropertie *SceneLoader::createStringPropertie(const QDomNode &propertyXml)
{
    QString label = propertyXml.attributes().namedItem("label").nodeValue();
    QString str = propertyXml.firstChildElement("str").text();

    StringPropertie *sp = new StringPropertie(label, str);
    return sp;
}

ColorPropertie *SceneLoader::createColorPropertie(const QDomNode &propertyXml)
{
    QString label = propertyXml.attributes().namedItem("label").nodeValue();
    int r = propertyXml.firstChildElement("red").text().toInt();
    int g = propertyXml.firstChildElement("green").text().toInt();
    int b = propertyXml.firstChildElement("blue").text().toInt();

    ColorPropertie *cp = new ColorPropertie(label, QColor(r, g, b));
    return cp;
}
