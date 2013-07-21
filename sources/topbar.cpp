#include "../../includes/topbar.h"

#include "../../includes/control.h"
#include "../../includes/fullscreenbutton.h"

#include <QtGui/QPainter>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDesktopWidget>

TopBar::TopBar(QWidget *parent) : QToolBar(parent)
{
    this->setFixedHeight(36);

    this->setMovable(false);
    this->setFloatable(false);

    mUrlField = new UrlField(this);

    mPressed = false;
    mPrevious = QPoint(0, 0);

    mControl = new Control(this);
    mControl->move(10, 12);

    mFullScreen = new FullScreenButton(this);
    mFullScreen->move(this->width() - 25, 12);

    mLeftButtonPressed = false;

    QGraphicsDropShadowEffect * e = new QGraphicsDropShadowEffect(this);
    e->setOffset(0, 2);
    e->setBlurRadius(15);
    e->setColor(QColor(0, 0, 0, .3 * 255));

    this->setGraphicsEffect(e);
}

UrlField *TopBar::getUrlField()
{
    return mUrlField;
}

void TopBar::toggleFullScreen(bool isFullScreen)
{
    if (isFullScreen)
    {
        mControl->hide();
        mFullScreen->hide();
    }
    else
    {
        mControl->show();
        mFullScreen->show();
    }
}

void TopBar::resizeEvent(QResizeEvent *)
{
    if (mFullScreen->isFullScreen())
    {
        mFullScreen->quitFullScreen();
    }

    mFullScreen->move(this->width() - 25, 12);

    mUrlField->resize(this->width() / 2, 25);
    mUrlField->move(this->width() / 2 - mUrlField->width() / 2, 6);

    this->resize(this->parentWidget()->width(), 0);
}

void TopBar::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        mFirstMove = true;
        mLeftButtonPressed = true;
        mPrevious = e->pos();
    }
}

void TopBar::mouseMoveEvent(QMouseEvent *e)
{
    if (mFullScreen->isFullScreen()) return;

    if (e->buttons() & Qt::LeftButton && mLeftButtonPressed)
    {
        QPoint diff = e->pos() - mPrevious;
        QPoint pos = this->parentWidget()->pos() + diff;

        QDesktopWidget d;
        if (pos.y() < d.availableGeometry().y())
            pos.setY(d.availableGeometry().y());

        this->parentWidget()->move(pos);
    }
}

void TopBar::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
        mLeftButtonPressed = false;
}

void TopBar::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);

    QLinearGradient g(0, 0, 0, this->height());
    g.setColorAt(0, QColor("#ffffff"));
    g.setColorAt(1, QColor("#e8e8e8"));
    p.setBrush(g);
    p.drawRoundedRect(this->rect().x() + 1, this->rect().y() + 1, this->rect().width() - 2, this->rect().height() - 2, 4, 4);
    p.drawRect(this->rect().x() + 1, this->rect().y() + 1 + 5, this->rect().width() - 2, this->rect().height() - 2 - 5);

    //p.setPen(QColor("#aaaaaa"));
    //p.drawLine(0, this->height(), this->width(), this->height());

    if (!mUrlField->isVisible())
    {
        QFont f = p.font();
        f.setPixelSize(14);

        p.setFont(f);
        p.translate(0, 1);
        p.setPen(QColor("#ffffff"));
        p.drawText(this->rect(), Qt::AlignCenter, QCoreApplication::applicationName().toUpper());

        p.translate(0, -1);
        p.setPen(QColor("#656565"));
        p.drawText(this->rect(), Qt::AlignCenter, QCoreApplication::applicationName().toUpper());
    }
}
