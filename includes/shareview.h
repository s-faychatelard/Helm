#ifndef SHAREVIEW_H
#define SHAREVIEW_H

#include <QtWidgets>

class ShareView : public QWidget
{
    Q_OBJECT
public:
    ShareView(QWidget *parent = 0);

    void showAnimated(bool animated = true);
    void hideAnimated(bool animated = true);
protected:
    void dropEvent(QDropEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void paintEvent(QPaintEvent *);
};

#endif // SHAREVIEW_H
