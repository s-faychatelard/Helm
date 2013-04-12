#ifndef TOPBAR_H
#define TOPBAR_H

#include <QToolBar>
#include <QMouseEvent>

#include "control.h"
#include "fullscreenbutton.h"
#include "urlfield.h"

class TopBar : public QToolBar
{
    Q_OBJECT
public:
    TopBar(QWidget *parent = 0);

    void toggleFullScreen(bool isFullScreen);

    UrlField* getUrlField();
    void resizeEvent(QResizeEvent *);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    bool mPressed;
    QPoint mPrevious;

    bool mLeftButtonPressed;
    bool mFirstMove;

    Control          *mControl;
    FullScreenButton *mFullScreen;
    UrlField         *mUrlField;
};

#endif // TOPBAR_H
