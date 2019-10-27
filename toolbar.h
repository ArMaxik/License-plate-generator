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

private:
   QAction *addItemAction;
   QAction *switchEditorsAction;
   QAction *randomizeAction;
   QAction *clearAllAction;
   QAction *saveSceneAction;
   QAction *loadSceneAction;
};

#endif // TOOLBAR_H
