#include "../../includes/webview.h"

#include "../../includes/application.h"
#include "../../includes/mainwindow.h"
#include "../../includes/shareview.h"
#include "../../includes/webviewcookie.h"

#include <QtGui/QGridLayout>

#include <QtCore/QDir>
#include <QtCore/QDebug>

static WebViewCookie *cookies = NULL;

WebView::WebView(QWidget *parent) : QWebView(parent)
{
    this->setAcceptDrops(true);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    this->settings()->setIconDatabasePath(Application::dataLocation() + "Icon/");
    this->settings()->setLocalStoragePath(Application::dataLocation() + "Local/");
    this->settings()->setOfflineStoragePath(Application::dataLocation() + "Offline/Data/");
    this->settings()->setOfflineWebApplicationCachePath(Application::dataLocation() + "Offline/Cache/");

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinish(bool)));
    //this->setStyleSheet("QWidget{ background: transparent;}");

    if (cookies == NULL)
        cookies = new WebViewCookie();

    this->page()->networkAccessManager()->setCookieJar(cookies);

    QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
    diskCache->setCacheDirectory(Application::dataLocation() + "Cache/");
    this->page()->networkAccessManager()->setCache(diskCache);
}

void WebView::takeScreenshot()
{
    //mWebPixmap = this->grab(this->rect());
}

void WebView::releaseScreenshot()
{
    mWebPixmap = QPixmap();
}

void WebView::loadFinish(bool success)
{
    Application::getWindow()->getTopBar()->getUrlField()->setProgress(0);

    if (!success)
    {
        QString search = Application::getWindow()->getTopBar()->getUrlField()->text();
        if (search.startsWith("http://"))
            search = search.remove("http://");
        this->load(QUrl("http://google.com/search?q=" + search));
    }

    this->page()->findText("TimeClock", QWebPage::HighlightAllOccurrences);
}

int WebView::focusedIndexElement()
{
    QWebElement document = this->page()->currentFrame()->documentElement();
    QWebElementCollection elements = document.findAll("input[type=text]");

    for(int i=0; i<elements.count(); i++)
    {
        QWebElement elem = elements.at(i);
        qDebug() << "Input elements : " << elem.toPlainText();
        if(elem.hasFocus())
        {
            qDebug() << "WebElement selected : " << i;
            return i;
        }
    }

    return -1;
}

void WebView::setFocusedElement(int index)
{
    if (index == -1)
    {
        this->parentWidget()->setFocus();
        return;
    }

    QWebElement document = this->page()->currentFrame()->documentElement();
    QWebElementCollection elements = document.findAll("input[type=text]");

    if (index >= elements.count()) return;

    this->setFocus();
    QWebElement elem = elements.at(index);
    qDebug() << "element.attribute value:" << elem.evaluateJavaScript("this.value").toString();
    elem.evaluateJavaScript("this.focusin()");
}

void WebView::wheelEvent(QWheelEvent *e)
{
    Application::getWindow()->wheelEvent(e);

    if (!Application::getWindow()->isAltPressed())
        QWebView::wheelEvent(e);
}

void WebView::dropEvent(QDropEvent *e)
{
    Application::getWindow()->getShareView()->hideAnimated();

    QWebView::dropEvent(e);
}

void WebView::dragEnterEvent(QDragEnterEvent *e)
{
    Application::getWindow()->getShareView()->showAnimated();

    e->accept();
}

void WebView::dragLeaveEvent(QDragLeaveEvent *e)
{
    if (this->mapFromGlobal(QCursor::pos()).y() > 0)
    {
        Application::getWindow()->getShareView()->hideAnimated();
    }

    e->accept();
}

void WebView::paintEvent(QPaintEvent *e)
{
    if (mWebPixmap.isNull())
        QWebView::paintEvent(e);

    if (this->width() != this->parentWidget()->width() - 2)
    {
        QPainter p(this);
        p.setPen(QColor("#aaaaaa"));

        if (!mWebPixmap.isNull())
        {
            p.drawPixmap(this->rect(), mWebPixmap.scaledToWidth(this->width(), Qt::SmoothTransformation));
        }

        p.drawLine(0, 0, this->width(), 0);
        p.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
        p.drawLine(0, this->height() - 1, this->width(), this->height() - 1);
        p.drawLine(0, 0, 0, this->height() - 1);

        p.end();
    }
}

