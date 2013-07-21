#include "../../includes/home.h"

#include <QtWidgets/QGridLayout>

Home::Home(QWidget *parent) : QWidget(parent)
{
    this->setContextMenuPolicy(Qt::NoContextMenu);

    QGridLayout *l = new QGridLayout(this);
    Q_UNUSED(l);
}

void Home::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    p.setBrush(QColor("#ff0000"));
    //p.drawRect(this->rect());
}
