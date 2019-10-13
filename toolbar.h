#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(const QString &title, QWidget *parent);

    QAction *getAddItemAction() const;
    QAction *getSwitchEditorsAction() const;
    QAction *getrandomizeAction() const;

private:
   QAction *addItemAction;
   QAction *switchEditorsAction;
   QAction *randomizeAction;
};

#endif // TOOLBAR_H
