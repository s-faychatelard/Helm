#ifndef TAB_H
#define TAB_H

#include <QtCore>
#include <QtWidgets>

#include "webview.h"

class TabItem : public QAbstractButton
{
    Q_OBJECT
public:
    TabItem(QWidget *parent = 0);
    void setSelected(bool = true);

protected:
    void paintEvent(QPaintEvent *);

private:
    bool mSelected;
};

class Tab : public QWidget
{
    Q_OBJECT
public:
    Tab(QWidget *parent = 0);

    void select(int index);
    void updateWithWebViews(QList<WebView*>*);

protected slots:
    void clicked();

private:
    QList<TabItem*> mItems;
};

#endif // TAB_H
