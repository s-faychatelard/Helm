#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "centralwidget.h"
#include "searchview.h"
#include "shareview.h"
#include "topbar.h"
#include "webcontainer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    ShareView* getShareView();
    SearchView* getSearchView();
    TopBar* getTopBar();
    WebContainer* getWebContainer();

    bool isAltPressed();

    void wheelEvent(QWheelEvent *);

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    CentralWidget *mCentralWidget;
    SearchView *mSearchView;
    ShareView *mShareView;
    TopBar * mTopBar;
    WebContainer * mWebContainer;

    bool mAltPressed;
    int mWheel;
};

#endif // MAINWINDOW_H
