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

private:
   QAction *addItemAction;
   QAction *switchEditorsAction;
};

#endif // TOOLBAR_H
