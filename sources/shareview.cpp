#include "../../includes/shareview.h"

#include <QtWidgets/QLayout>
#include <QtGui/QPainter>
#include <QtGui/QDropEvent>
#include <QtGui/QDragEnterEvent>

#include <QtCore/QDebug>
#include <QtCore/QPropertyAnimation>

#define ANIMATION_DURATION 160

ShareView::ShareView(QWidget *parent) : QWidget(parent)
{
    this->setAcceptDrops(true);
    this->setMaximumHeight(101);
}

void ShareView::showAnimated(bool animated)
{
    if (this->isVisible()) return;

    if (animated)
    {
        this->resize(this->parentWidget()->width(), this->height());
        this->show();

        QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumHeight");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(1);
        animation->setEndValue(101);
        animation->start();
    }
    else
    {
        this->setMaximumHeight(101);
        this->resize(this->parentWidget()->width(), this->height());
        this->show();
    }
}

void ShareView::hideAnimated(bool animated)
{
    this->setCursor(Qt::ArrowCursor);

    if (animated)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumHeight");
        connect(animation, SIGNAL(finished()), this, SLOT(hide()));
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(101);
        animation->setEndValue(1);
        animation->start();
    }
    else
    {
        this->hide();
    }
}

void ShareView::dropEvent(QDropEvent *e)
{
    qDebug() << "ShareView::dropEvent";

    this->hideAnimated();

    e->accept();
}

void ShareView::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
}

void ShareView::dragLeaveEvent(QDragLeaveEvent *e)
{
    if (this->mapFromGlobal(QCursor::pos()).y() < this->height())
    {
        this->hideAnimated();
    }

    e->accept();
}

void ShareView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    QRect rect = this->rect();
    p.setBrush(QColor("#4b5055"));
    p.drawRect(rect);

    rect.setX(rect.x() + 1);
    rect.setY(rect.y() + 1);
    rect.setWidth(rect.width() - 2);
    rect.setHeight(rect.height() - 2);

    p.setBrush(QPixmap(":/sharebg.png"));
    p.drawRect(rect);

    QLinearGradient g(0, 0, 0, this->height());
    g.setColorAt(0, QColor("#4d5257"));
    g.setColorAt(1, QColor("#7a8289"));
    p.setBrush(g);

    p.setOpacity(.7);
    p.drawRect(rect);

    p.setPen(QColor("#ffffff"));
    p.setOpacity(1.);
    p.drawLine(0, this->height() - 2, this->width(), this->height() - 2);

    p.setPen(QColor("#e8e8e8"));
    p.drawLine(0, this->height() - 1, this->width(), this->height() - 1);
}
