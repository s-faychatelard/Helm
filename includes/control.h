#ifndef CONTROL_H
#define CONTROL_H

#include <QtWidgets>

#include "windowbutton.h"

class Control : public QWidget
{
    Q_OBJECT
public:
    Control(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    WindowButton *close;
    WindowButton *minimize;
    WindowButton *resize;
};

#endif // CONTROL_H
