#include "../../includes/control.h"

Control::Control(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(55, 12);

    close = new WindowButton(WindowButton::CLOSE, this);
    close->move(0, 0);

    minimize = new WindowButton(WindowButton::MINIMIZE, this);
    minimize->move(20, 0);

    resize = new WindowButton(WindowButton::RESIZE, this);
    resize->move(40, 0);
}

void Control::enterEvent(QEvent *)
{
    close->setHover(true);
    minimize->setHover(true);
    resize->setHover(true);

    this->repaint();
}

void Control::leaveEvent(QEvent *)
{
    close->setHover(false);
    minimize->setHover(false);
    resize->setHover(false);

    this->repaint();
}
