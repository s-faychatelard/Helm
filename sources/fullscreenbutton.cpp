#include "../../includes/fullscreenbutton.h"

#include "../../includes/topbar.h"

#include <QtGui/QPainter>

#if defined(Q_OS_MAC)
#include "../../includes/others/mac/mac.h"
#endif

FullScreenButton::FullScreenButton(QWidget *parent) : QAbstractButton(parent)
{
    this->setFixedSize(14, 14);
    this->setAttribute(Qt::WA_NoMousePropagation, true);

    connect(this, SIGNAL(clicked()), this, SLOT(toggleFullScreen()));

    mHover = false;
    mFullScreen = false;

#if !defined(Q_OS_MAC)
    this->hide();
#endif
}

FullScreenButton::~FullScreenButton()
{
    disconnect(this, SIGNAL(clicked()), 0, 0);
}

void FullScreenButton::enterEvent(QEvent *)
{
    mHover = true;

    this->repaint();
}

void FullScreenButton::leaveEvent(QEvent *)
{
    mHover = false;

    this->repaint();
}


bool FullScreenButton::isFullScreen()
{
    return mFullScreen;
}

void FullScreenButton::toggleFullScreen()
{
    TopBar* t = (TopBar*)this->parentWidget();
    t->toggleFullScreen(true);

#if defined(Q_OS_MAC)
    Mac m(this);
    m.enterFullscreenMode();
#endif

    mFullScreen = true;
}

void FullScreenButton::quitFullScreen()
{
    mFullScreen = false;

    TopBar* t = (TopBar*)this->parentWidget();
    t->toggleFullScreen(false);
}

void FullScreenButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);

    if (this->isDown())
        p.drawPixmap(this->rect(), QPixmap(":/fullscreen-pressed.png"));
    else if (mHover)
        p.drawPixmap(this->rect(), QPixmap(":/fullscreen-hover.png"));
    else
        p.drawPixmap(this->rect(), QPixmap(":/fullscreen.png"));
}
