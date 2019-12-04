#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(const QString &title, QWidget *parent);

    QAction *getAddItemAction() const       { return addItemAction; }
    QAction *getSwitchEditorsAction() const { return switchEditorsAction; }
    QAction *getrandomizeAction() const     { return randomizeAction; }
    QAction *getClearAllAction() const      { return clearAllAction; }
    QAction *getSaveSceneAction() const     { return saveSceneAction; }
    QAction *getLoadSceneAction() const     { return loadSceneAction; }

private:
   QAction *addItemAction;
   QAction *switchEditorsAction;
   QAction *randomizeAction;
   QAction *clearAllAction;
   QAction *saveSceneAction;
   QAction *loadSceneAction;

   bool actionsVisible;

private slots:
    void toggleEditors();
};

#endif // TOOLBAR_H
