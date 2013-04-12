#include "../../includes/windowbutton.h"

#include "../../includes/application.h"
#include "../../includes/mainwindow.h"

#include <QtGui/QPainter>

WindowButton::WindowButton(WindowButtonType t, QWidget *parent) : QAbstractButton(parent)
{
    this->setFixedSize(12, 12);
    this->setAttribute(Qt::WA_NoMousePropagation, true);

    connect(this, SIGNAL(clicked()), this, SLOT(pressed()));

    mType = t;
    mHover = false;
}

WindowButton::~WindowButton()
{
    disconnect(this, SIGNAL(clicked()), 0, 0);
}

void WindowButton::setHover(bool hover)
{
    mHover = hover;
}

void WindowButton::pressed()
{
    switch (mType) {
        case CLOSE:
        {
            Application::getWindow()->close();
            break;
        }
        case MINIMIZE:
        {
            Application::getWindow()->showMinimized();
            break;
        }
        case RESIZE:
        {
            MainWindow *w = Application::getWindow();
            if (w->isMaximized())
                w->showNormal();
            else
                w->showMaximized();
            break;
        }
    }
}

void WindowButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);

    MainWindow *w = Application::getWindow();

    if (this->isDown())
    {
        switch (mType) {
        case CLOSE:
            p.drawPixmap(this->rect(), QPixmap(":/close-hover.png"));
            break;
        case MINIMIZE:
            p.drawPixmap(this->rect(), QPixmap(":/minimize-hover.png"));
            break;
        case RESIZE:
            p.drawPixmap(this->rect(), QPixmap(":/resize-hover.png"));
            break;
        }

        p.setBrush(QColor(0, 0, 0, 60));
        p.drawRoundedRect(this->rect(), 8, 8);
    }
    else if (w->isActiveWindow())
    {
        switch (mType) {
        case CLOSE:
            p.drawPixmap(this->rect(), QPixmap(":/close-hover.png"));
            break;
        case MINIMIZE:
            p.drawPixmap(this->rect(), QPixmap(":/minimize-hover.png"));
            break;
        case RESIZE:
            p.drawPixmap(this->rect(), QPixmap(":/resize-hover.png"));
            break;
        }
    }
    else
        p.drawPixmap(this->rect(), QPixmap(":/window.png"));
}
