#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(const QString &title, QWidget *parent);

    QAction *getAddStaticImageAction() const;
    QAction *getAddStaticTextAction() const;

private:
   QAction *addStaticImageAction;
   QAction *addStaticTextAction;
};

#endif // TOOLBAR_H
