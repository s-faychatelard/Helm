#include "../../includes/webcontainer.h"

#include "../../includes/application.h"
#include "../../includes/mainwindow.h"
#include "../../includes/urlfield.h"

#include <QtGui/QGridLayout>
#include <QtGui/QDesktopServices>

#include <QtCore/QDebug>
#include <QtCore/QPropertyAnimation>

WebContainer::WebContainer(QWidget *parent) : QWidget(parent)
{
    mCurrentWebView = -1;
    mLastFocusedIndexElement = -1;

    mWebViews = new QList<WebView*>();

    mTab = new Tab(this);
    mTab->hide();

    mHome = new Home(this);
    mHome->show();

    MainWindow *m = (MainWindow*)this->parentWidget()->parentWidget();
    m->getTopBar()->getUrlField()->hide();
}

WebContainer::~WebContainer()
{
    this->disconnectCurrentWebView();
}

WebView* WebContainer::getWebView()
{
    if (mCurrentWebView == -1) return NULL;

    return mWebViews->at(mCurrentWebView);
}

void WebContainer::showHome()
{
    if (mHome->isVisible()) return;

    QPropertyAnimation *animation = new QPropertyAnimation(mTab, "geometry");
    connect(animation, SIGNAL(finished()), mTab, SLOT(hide()));
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mTab->geometry());
    animation->setEndValue(QRect(this->width(), 0, this->width(), 0));
    animation->start();

    if (mCurrentWebView != -1)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView), "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(mWebViews->at(mCurrentWebView)->geometry());
        animation->setEndValue(QRect(this->width() + 100, MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();
    }

    if (mCurrentWebView + 1 < mWebViews->count())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView + 1), "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(mWebViews->at(mCurrentWebView + 1)->geometry());
        animation->setEndValue(QRect(this->width() + 100, MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();
    }

    mHome->show();
    mHome->raise();

    Application::getWindow()->getTopBar()->getUrlField()->hide();

    animation = new QPropertyAnimation(mHome, "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(QRect(-this->width(), 0, this->width() - 2, this->height() - 1));
    animation->setEndValue(QRect(1, 0, this->width() - 2, this->height() - 1));
    animation->start();

    mHome->setFocus();

    mCurrentWebView = -1;
}

void WebContainer::hideHome()
{
    QPropertyAnimation *animation = new QPropertyAnimation(mTab, "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(QRect(this->width(), 0, this->width(), 0));
    animation->setEndValue(QRect(0, 0, this->width(), 0));
    animation->start();
    mTab->show();

    if (mCurrentWebView != -1)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView), "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(mWebViews->at(mCurrentWebView)->geometry());
        animation->setEndValue(QRect(this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();
    }

    Application::getWindow()->getTopBar()->getUrlField()->show();

    animation = new QPropertyAnimation(mHome, "geometry");
    connect(animation, SIGNAL(finished()), mHome, SLOT(hide()));
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mHome->geometry());
    animation->setEndValue(QRect(-this->width(), 0, this->width() - 2, this->height() - 1));
    animation->start();
}

void WebContainer::disconnectCurrentWebView()
{
    if (mCurrentWebView == -1 || mWebViews->at(mCurrentWebView) == NULL) return;

    disconnect(mWebViews->at(mCurrentWebView), SIGNAL(iconChanged()), 0, 0);
    disconnect(mWebViews->at(mCurrentWebView), SIGNAL(linkClicked(QUrl)), 0, 0);
    disconnect(mWebViews->at(mCurrentWebView), SIGNAL(loadFinished(bool)), 0, 0);

    disconnect(mWebViews->at(mCurrentWebView), SIGNAL(urlChanged(QUrl)), 0, 0);
    disconnect(mWebViews->at(mCurrentWebView), SIGNAL(titleChanged(QString)), 0, 0);
    disconnect(mWebViews->at(mCurrentWebView), SIGNAL(loadProgress(int)), 0, 0);
}

void WebContainer::urlChange(QUrl url)
{
    Application::getWindow()->getTopBar()->getUrlField()->urlChanged(url);
    mWebViews->at(mCurrentWebView)->urlChanged(url);
}

void WebContainer::newTab(QString url)
{
    mUrlToLoad = url;

    WebView *webView = new WebView(this);
    webView->hide();

    mWebViews->push_back(webView);

    this->disconnectCurrentWebView();

    MainWindow* m = Application::getWindow();
    if (m->getTopBar() != NULL)
    {
        connect(webView, SIGNAL(iconChanged()), this, SLOT(iconChange()));
        connect(webView, SIGNAL(linkClicked(QUrl)), this, SLOT(urlChange(QUrl)));
        connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinish(bool)));

        connect(webView, SIGNAL(urlChanged(QUrl)), m->getTopBar()->getUrlField(), SLOT(urlChanged(QUrl)));
        connect(webView, SIGNAL(titleChanged(QString)), m->getTopBar()->getUrlField(), SLOT(setTitle(QString)));
        connect(webView, SIGNAL(loadProgress(int)), m->getTopBar()->getUrlField(), SLOT(setProgress(int)));
    }

    if (mCurrentWebView == -1)
    {
        if (mHome->isVisible())
            this->hideHome();

        this->switchToTab(mWebViews->indexOf(webView));

        this->loadUrl(mUrlToLoad);

        if (m->getTopBar() != NULL)
        {
            m->getTopBar()->getUrlField()->setIcon(QPixmap(":/favicon.png"));
        }
    }
    else
    {
        mNewTabIndex = mWebViews->indexOf(webView);

        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView), "geometry");
        connect(animation, SIGNAL(finished()), this, SLOT(newTabAnimation()));
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(mWebViews->at(mCurrentWebView)->geometry());
        animation->setEndValue(QRect(MARGIN, MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();
    }
}

void WebContainer::newTabAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView), "geometry");
    connect(animation, SIGNAL(finished()), this, SLOT(newTabAnimation2()));
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mWebViews->at(mCurrentWebView)->geometry());
    animation->setEndValue(QRect(-this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
    animation->start();

    mWebViews->at(mNewTabIndex)->setGeometry(this->width() - VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2);
    mWebViews->at(mNewTabIndex)->show();

    animation = new QPropertyAnimation(mWebViews->at(mNewTabIndex), "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mWebViews->at(mNewTabIndex)->geometry());
    animation->setEndValue(QRect(MARGIN, MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
    animation->start();
}

void WebContainer::newTabAnimation2()
{
    QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mNewTabIndex), "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mWebViews->at(mNewTabIndex)->geometry());
    animation->setEndValue(QRect(1, 0, this->width() - 2, this->height() - 1));
    animation->start();

    mCurrentWebView = mNewTabIndex;

    this->loadUrl(mUrlToLoad);

    MainWindow* m = Application::getWindow();
    if (m->getTopBar() != NULL)
    {
        m->getTopBar()->getUrlField()->setIcon(QPixmap(":/favicon.png"));
    }

    mNewTabIndex = -1;
}

void WebContainer::removeTab()
{
    if (mCurrentWebView == -1) return;

    this->disconnectCurrentWebView();

    QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView), "geometry");
    connect(animation, SIGNAL(finished()), mWebViews->at(mCurrentWebView), SLOT(hide()));
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mWebViews->at(mCurrentWebView)->geometry());

    animation->setEndValue(QRect(mWebViews->at(mCurrentWebView)->geometry().x(), this->height(),
                                 mWebViews->at(mCurrentWebView)->geometry().width(),
                                 mWebViews->at(mCurrentWebView)->geometry().height()));
    animation->start();

    mWebViews->removeAt(mCurrentWebView);

    if (mCurrentWebView - 1 >= 0)
        this->switchToPreviousTab();
    else if (mCurrentWebView < mWebViews->count())
    {
        this->switchAnimated(-1, mCurrentWebView);

        MainWindow* m = Application::getWindow();
        m->getTopBar()->getUrlField()->setTitle(mWebViews->at(mCurrentWebView)->title());
        m->getTopBar()->getUrlField()->setText(mWebViews->at(mCurrentWebView)->url().toString());
        m->getTopBar()->getUrlField()->setIcon(mWebViews->at(mCurrentWebView)->icon().pixmap(16, 16));
    }
    else
    {
        mCurrentWebView = -1;

        this->showHome();

        MainWindow* m = Application::getWindow();
        m->getTopBar()->getUrlField()->setText("");
        m->getTopBar()->getUrlField()->setTitle("");
        m->getTopBar()->getUrlField()->setIcon(QPixmap());
    }

    mTab->updateWithWebViews(mWebViews);
    mTab->select(mCurrentWebView);
}

void WebContainer::showTab(int index)
{
    for(int i=0; i<mWebViews->count(); i++)
    {
        mWebViews->at(i)->hide();
    }

    MainWindow* m = Application::getWindow();

    mWebViews->at(index)->show();

    if (!m->isAltPressed())
    {
        mWebViews->at(index)->setGeometry(QRect(2, 2, this->width() - 4, this->height() - 4));
        return;
    }

    int hasLeft = 0;
    int hasRight = 0;

    if (index - 1 >= 0)
    {
        mWebViews->at(index - 1)->show();

        int offset = 0;
        if (m->isAltPressed())
            offset = LEFT_OFFSET;

        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(index - 1), "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(QRect(-this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->setEndValue(QRect(-this->width() + offset + VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();

        hasLeft = VISIBLE_MARGIN;
    }

    if (index + 1 < mWebViews->count())
    {
        mWebViews->at(index + 1)->show();

        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(index + 1), "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(QRect(this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->setEndValue(QRect(this->width() - VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();

        hasRight = VISIBLE_MARGIN;
    }

    QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(index), "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(QRect(2, 2, this->width() - 4, this->height() - 4));
    animation->setEndValue(QRect(MARGIN + hasLeft, MARGIN, this->width() - MARGIN*2 - hasLeft - hasRight, this->height() - MARGIN*2));
    animation->start();

    mTab->select(index);
}

void WebContainer::switchAnimated(int previousTab, int newTab)
{
    for(int i=0; i<mWebViews->count(); i++)
    {
        mWebViews->at(i)->hide();
    }

    MainWindow* m = Application::getWindow();

    if (!m->isAltPressed())
    {
        if (previousTab != -1)
            mWebViews->at(previousTab)->hide();

        mWebViews->at(newTab)->show();
        mWebViews->at(newTab)->setGeometry(QRect(1, 0, this->width() - 2, this->height() - 1));

        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(newTab), "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(QRect(this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->setEndValue(QRect(1, 0, this->width() - 2, this->height() - 1));
        animation->start();

        return;
    }

    int offset = 0;
    if (m->isAltPressed())
        offset = LEFT_OFFSET;

    int hasLeft = 0;
    int hasRight = 0;

    if (previousTab < newTab)
    {
        if (previousTab != -1)
        {
            mWebViews->at(previousTab)->show();

            QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(previousTab), "geometry");
            animation->setDuration(ANIMATION_DURATION);
            animation->setStartValue(mWebViews->at(previousTab)->geometry());
            animation->setEndValue(QRect(-this->width() + offset + VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
            animation->start();

            hasLeft = VISIBLE_MARGIN;
        }

        if (newTab + 1 < mWebViews->count())
        {
            mWebViews->at(newTab + 1)->show();

            QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(newTab + 1), "geometry");
            animation->setDuration(ANIMATION_DURATION);
            animation->setStartValue(mWebViews->at(newTab + 1)->geometry());
            animation->setEndValue(QRect(this->width() - VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
            animation->start();

            hasRight = VISIBLE_MARGIN;
        }
    }
    else
    {
        if (previousTab != -1)
        {
            mWebViews->at(previousTab)->show();

            QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(previousTab), "geometry");
            animation->setDuration(ANIMATION_DURATION);
            animation->setStartValue(mWebViews->at(previousTab)->geometry());
            animation->setEndValue(QRect(this->width() - VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
            animation->start();

            hasRight = VISIBLE_MARGIN;
        }

        if (newTab - 1 >= 0)
        {
            mWebViews->at(newTab - 1)->show();

            QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(newTab - 1), "geometry");
            animation->setDuration(ANIMATION_DURATION);
            animation->setStartValue(mWebViews->at(newTab - 1)->geometry());
            animation->setEndValue(QRect(-this->width() + offset + VISIBLE_MARGIN , MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
            animation->start();

            hasLeft = VISIBLE_MARGIN;
        }
    }

    mWebViews->at(newTab)->show();
    mWebViews->at(newTab)->raise();

    QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(newTab), "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mWebViews->at(newTab)->geometry());
    animation->setEndValue(QRect(MARGIN + hasLeft, MARGIN, this->width() - MARGIN*2 - hasLeft - hasRight, this->height() - MARGIN*2));
    animation->start();

    mTab->select(newTab);
}

void WebContainer::switchToTab(int index)
{
    for(int i=0; i<mWebViews->count(); i++)
    {
        mWebViews->at(i)->hide();
    }

    if (mCurrentWebView >= mWebViews->count())
        mCurrentWebView = -1;

    if (mCurrentWebView != -1 && mCurrentWebView < mWebViews->count())
        mWebViews->at(mCurrentWebView)->hide();

    int previous = mCurrentWebView;
    mCurrentWebView = index;

    if (mCurrentWebView != previous)
    {
        this->switchAnimated(previous, mCurrentWebView);
    }
    else
    {
        this->showTab(mCurrentWebView);
    }

    MainWindow* m = Application::getWindow();
    m->getTopBar()->getUrlField()->setTitle(mWebViews->at(mCurrentWebView)->title());
    m->getTopBar()->getUrlField()->setText(mWebViews->at(mCurrentWebView)->url().toString());
    m->getTopBar()->getUrlField()->setIcon(mWebViews->at(mCurrentWebView)->icon().pixmap(16, 16));
}

void WebContainer::switchToNextTab()
{
    if (mCurrentWebView + 1 < mWebViews->count() && mHome->isVisible())
    {
        this->hideHome();
    }

    if (mCurrentWebView + 1 < mWebViews->count())
        this->switchToTab(mCurrentWebView + 1);
}

void WebContainer::switchToPreviousTab()
{
    if (mCurrentWebView - 1 >= 0)
        this->switchToTab(mCurrentWebView - 1);
    else
        this->showHome();
}

void WebContainer::showSwitcher()
{
    mTab->updateWithWebViews(mWebViews);

    for(int i=0; i<mWebViews->count(); i++)
    {
        mWebViews->at(i)->hide();
        mWebViews->at(i)->takeScreenshot();
    }

    if (mCurrentWebView == -1) return;

    mLastFocusedIndexElement = mWebViews->at(mCurrentWebView)->focusedIndexElement();

    this->switchToTab(mCurrentWebView);

    this->setFocus();
}

void WebContainer::hideSwitcher()
{
    for(int i=0; i<mWebViews->count(); i++)
    {
        mWebViews->at(i)->hide();
    }

    if (mCurrentWebView == -1) return;

    if (mCurrentWebView - 1 >= 0)
    {
        mWebViews->at(mCurrentWebView - 1)->show();

        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView - 1), "geometry");
        connect(animation, SIGNAL(finished()), mWebViews->at(mCurrentWebView - 1), SLOT(hide()));
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(mWebViews->at(mCurrentWebView - 1)->geometry());
        animation->setEndValue(QRect(-this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();
    }

    if (mCurrentWebView + 1 < mWebViews->count())
    {
        mWebViews->at(mCurrentWebView + 1)->show();

        QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView + 1), "geometry");
        connect(animation, SIGNAL(finished()), mWebViews->at(mCurrentWebView + 1), SLOT(hide()));
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(mWebViews->at(mCurrentWebView + 1)->geometry());
        animation->setEndValue(QRect(this->width(), MARGIN, this->width() - MARGIN*2, this->height() - MARGIN*2));
        animation->start();
    }

    mWebViews->at(mCurrentWebView)->show();

    QPropertyAnimation *animation = new QPropertyAnimation(mWebViews->at(mCurrentWebView), "geometry");
    connect(animation, SIGNAL(finished()), this, SLOT(releaseScreenshots()));
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(mWebViews->at(mCurrentWebView)->geometry());
    animation->setEndValue(QRect(1, 0, this->width() - 2, this->height() - 1));
    animation->start();

    mWebViews->at(mCurrentWebView)->setFocusedElement(mLastFocusedIndexElement);
}

void WebContainer::releaseScreenshots()
{
    for(int i=0; i<mWebViews->count(); i++)
    {
        mWebViews->at(i)->releaseScreenshot();
    }
}

void WebContainer::resizeEvent(QResizeEvent *)
{
    if (mCurrentWebView != -1)
        mWebViews->at(mCurrentWebView)->setGeometry(QRect(1, 0, this->width() - 2, this->height() - 1));

    mHome->setGeometry(QRect(1, 0, this->width() - 2, this->height() - 1));

    mTab->resize(this->width(), 0);
}

void WebContainer::keyPressEvent(QKeyEvent *e)
{
    if (mCurrentWebView != -1 && mWebViews->at(mCurrentWebView)->focusedIndexElement() == -1)
    {
        if (e->key() == Qt::Key_Alt || e->key() == Qt::Key_AltGr ||
                e->key() == Qt::Key_Shift || e->key() == Qt::Key_Meta ||
                e->key() == Qt::Key_Control)
        {
            QWidget::keyPressEvent(e);
            return;
        }

        MainWindow* m = Application::getWindow();
        m->getTopBar()->getUrlField()->setText(e->text());
        m->getTopBar()->getUrlField()->setFocus();
    }

    QWidget::keyPressEvent(e);
}

void WebContainer::loadFinish(bool)
{
    Application::getWindow()->getTopBar()->getUrlField()->setProgress(0);
}

void WebContainer::iconChange()
{
    if (mCurrentWebView == -1) return;

    Application::getWindow()->getTopBar()->getUrlField()->setIcon(mWebViews->at(mCurrentWebView)->icon().pixmap(16, 16));
}

void WebContainer::loadUrl(QString url)
{
    if (!url.contains("://"))
    {
        url = "http://" + url;
    }

    if (mCurrentWebView == -1)
    {
        this->newTab();
        mWebViews->at(mCurrentWebView)->setGeometry(QRect(1, 0, this->width() - 2, this->height() - 1));
    }

    Application::getWindow()->getTopBar()->getUrlField()->setText(url);

    mWebViews->at(mCurrentWebView)->load(QUrl(url));
}
