#ifndef FULLSCREENBUTTON_H
#define FULLSCREENBUTTON_H

#include <QAbstractButton>

class FullScreenButton : public QAbstractButton
{
    Q_OBJECT
public:
    FullScreenButton(QWidget *parent = 0);
    ~FullScreenButton();

    void quitFullScreen();
    bool isFullScreen();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

protected slots:
    void toggleFullScreen();

private:
    bool mHover;
    bool mFullScreen;
};

#endif // FULLSCREENBUTTON_H
