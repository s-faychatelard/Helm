#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QtWidgets/QScrollBar>

class ScrollBar : public QScrollBar
{
	Q_OBJECT
public:
    ScrollBar(Qt::Orientation pOrientation = Qt::Vertical, QWidget * w = 0);

    void wheelEvent(QWheelEvent * e);
protected:
    void paintEvent(QPaintEvent * e);
};

#endif
