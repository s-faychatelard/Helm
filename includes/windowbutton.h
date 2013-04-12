#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <QAbstractButton>

class WindowButton : public QAbstractButton
{
    Q_OBJECT
public:
    enum WindowButtonType {
        CLOSE,
        MINIMIZE,
        RESIZE
    };

    WindowButton(WindowButtonType t, QWidget *parent = 0);
    ~WindowButton();

    void setHover(bool hover);

protected:
    void paintEvent(QPaintEvent *);

protected slots:
    void pressed();

private:
    bool mHover;
    WindowButtonType mType;
};

#endif // WINDOWBUTTON_H
