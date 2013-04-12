#ifndef HOME_H
#define HOME_H

#include <QtWidgets>

class Home : public QWidget
{
    Q_OBJECT
public:
    Home(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // HOME_H
