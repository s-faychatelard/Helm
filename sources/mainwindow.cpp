#include "../../includes/mainwindow.h"

#include <QtWidgets/QMenuBar>
#include <QtGui/QPainter>
#include <QtWidgets/QGridLayout>

#if defined(Q_OS_DARWIN)
#include "../../includes/others/mac/mac.h"
#elif defined(Q_OS_WIN)
#include "../../includes/others/windows/windows.h"
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    mAltPressed = false;
    mWheel = 0;

    mTopBar = NULL;
    mWebContainer = NULL;

    this->setMinimumSize(1000, 600);
    this->setContextMenuPolicy(Qt::NoContextMenu);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);

    mCentralWidget = new CentralWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mCentralWidget);
    layout->setMargin(0);
    layout->setSpacing(0);

    mSearchView = new SearchView(this);
    mTopBar = new TopBar(this);
    mWebContainer = new WebContainer(mCentralWidget);
    mShareView = new ShareView(mCentralWidget);

#if defined(Q_OS_DARWIN)
    Mac m(this);
    //m.registerAsDefaultBrowser();
    Q_UNUSED(m);
#elif defined(Q_OS_WIN)
    Windows w(this);
    Q_UNUSED(w);
#endif

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menu = new QMenu(tr("File"), this);

    QAction * newTab = new QAction(tr("New tab"), this);
    newTab->setShortcuts(QKeySequence::AddTab);
    menu->addAction(newTab);
    connect(newTab, SIGNAL(triggered()), mWebContainer, SLOT(newTab()));
    menuBar->addMenu(menu);

    QAction * closeTab = new QAction(tr("Close tab"), this);
    closeTab->setShortcuts(QKeySequence::Close);
    menu->addAction(closeTab);
    connect(closeTab, SIGNAL(triggered()), mWebContainer, SLOT(removeTab()));
    menuBar->addMenu(menu);

    this->setCentralWidget(mCentralWidget);
    this->addToolBar(mTopBar);
    this->setMenuBar(menuBar);
    this->setFocus();

    layout->addWidget(mShareView);
    layout->addWidget(mWebContainer);

    mShareView->hide();
    mSearchView->hide();

    mSearchView->raise();

    mWebContainer->setFocus();
}

ShareView* MainWindow::getShareView()
{
    return mShareView;
}

SearchView* MainWindow::getSearchView()
{
    return mSearchView;
}

TopBar* MainWindow::getTopBar()
{
    return mTopBar;
}

WebContainer* MainWindow::getWebContainer()
{
    return mWebContainer;
}

bool MainWindow::isAltPressed()
{
    return mAltPressed;
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    QMainWindow::wheelEvent(e);

    if (!mAltPressed)
    {
        mWheel = 0;
        return;
    }

    if (mWheel > 0 && e->delta() < 0)
        mWheel = 0;
    else if (mWheel < 0 && e->delta() > 0)
        mWheel = 0;

    mWheel += e->delta()/40;

    /*qDebug() << "mWheel : " << mWheel;

    if (mWheel < -40)
    {
        mWebContainer->switchToNextTab();
        mWheel = 0;
    }
    else if (mWheel > 40)
    {
        mWebContainer->switchToPreviousTab();
        mWheel = 0;
    }*/
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == 0 && (e->key() == Qt::Key_Alt || e->key() == Qt::Key_AltGr))
    {
        mAltPressed = true;

        mWebContainer->showSwitcher();
    }
    else if ((e->modifiers() == Qt::AltModifier || e->modifiers() == (Qt::AltModifier | Qt::KeypadModifier)))
    {
        if (e->key() == Qt::Key_Right)
        {
            mWebContainer->switchToNextTab();
        }
        else if (e->key() == Qt::Key_Left)
        {
            mWebContainer->switchToPreviousTab();
        }
        else if (e->key() == Qt::Key_Down)
        {
            mWebContainer->removeTab();
        }
    }

    QWidget::keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (mAltPressed && (e->key() == Qt::Key_Alt || e->key() == Qt::Key_AltGr))
    {
        mWebContainer->hideSwitcher();

        mAltPressed = false;
    }

    QWidget::keyReleaseEvent(e);
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    mTopBar->resizeEvent(e);

    mShareView->move(1, 10);
    mShareView->resize(this->width() - 2, 45);

    mSearchView->resize(this->width() / 2 + 2, 200);
    mSearchView->move(this->width() / 2 - mSearchView->width() / 2, 32);

    QMainWindow::resizeEvent(e);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);

    p.setBrush(QColor("#aaaaaa"));
    p.drawRoundedRect(this->rect(), 4, 4);

    //p.setBrush(QColor("#e7e6e4"));
    p.setBrush(QPixmap(":/mainbg.png"));
    p.drawRoundedRect(this->rect().x() + 1, this->rect().y() + 1, this->width() - 2, this->height() - 2, 3, 3);
}
