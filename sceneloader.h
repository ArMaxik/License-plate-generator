#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <QObject>
#include <QDomDocument>
#include "textureeditorwidget.h"

class SceneLoader : public QObject
{
    Q_OBJECT
public:
    SceneLoader(TextureEditorWidget *textureEditor, QObject *parent = nullptr);

signals:

public slots:
    void loadScene();

private:
    TextureEditorWidget *texEditor;
    BasicItem *createItem(const QDomNode &BasicItemXml);
    BasicNode *createNode(const QDomNode &nodeXml, BoundRect *br, BasicChanel::Nodes *nodeType);
    NumberProperty *createNumberPropertie(const QDomNode &propertyXml);
    ImageProperty *createImagePropertie(const QDomNode &propertyXml);
    StringProperty *createStringPropertie(const QDomNode &propertyXml);
    ColorProperty *createColorPropertie(const QDomNode &propertyXml);
};

#endif // SCENELOADER_H
