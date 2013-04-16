#include "../../includes/urlfield.h"

#include "../../includes/application.h"
#include "../../includes/mainwindow.h"

#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>

UrlField::UrlField(QWidget *parent) : QLineEdit(parent)
{
    this->setFixedHeight(26);

    this->setPlaceholderText("URL : Search : History");
    this->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(this, SIGNAL(editingFinished()), this, SLOT(editingEnd()));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChange()));
    connect(this, SIGNAL(returnPressed()), this, SLOT(returnPress()));

    QPalette palette;
    palette.setColor(QPalette::Text, QColor("#393939"));
    //palette.setColor(QPalette::Highlight, QColor("#c0c0c0"));
    palette.setColor(QPalette::HighlightedText, QColor("#393939"));
    this->setPalette(palette);

    this->setStyleSheet("QLineEdit{ background: transparent; border: none; margin-left: 10px; margin-right: 10px; }");

    mProgress = 0;
}

void UrlField::editingEnd()
{
    Application::getWindow()->getSearchView()->hide();
}

void UrlField::updateLeftOffset()
{
    int offset = 10;
    if (!mIcon.isNull())
    {
        offset += 12 + 10;
    }
    if (!mTitle.isEmpty())
    {
        QFontMetrics metrics = this->fontMetrics();
        offset += metrics.width(mTitle) + 5;
        offset += metrics.width(QChar(0x2022)) + 5;
    }

    QString style = "QLineEdit{ background: transparent; border: none; margin-left:";
    style += QString::number(offset);
    style += "px; margin-right: 10px; }";
    this->setStyleSheet(style);
}

void UrlField::setProgress(int progress)
{
    mProgress = progress;

    this->repaint();
}

void UrlField::setText(const QString &t)
{
    QLineEdit::setText(t);
    this->clearFocus();

    this->updateLeftOffset();
}

void UrlField::setIcon(QPixmap i)
{
    if (i.isNull())
    {
        mIcon = QPixmap(":/favicon.png").scaledToWidth(16, Qt::SmoothTransformation);
        return;
    }
    mIcon = i.scaledToWidth(16, Qt::SmoothTransformation);

    this->updateLeftOffset();
}

void UrlField::setTitle(QString t)
{
#define MAX_TITLE_LENGTH 30
    if (t.length() > MAX_TITLE_LENGTH)
    {
        mTitle = t;

        if (t.contains("-"))
        {
            mTitle = t.left(t.indexOf("-"));
        }
        else if (t.contains("|"))
        {
            mTitle = t.left(t.indexOf("|"));
        }

        if (mTitle.length() > MAX_TITLE_LENGTH)
        {
            mTitle = t.left(MAX_TITLE_LENGTH - 3);
            mTitle += "...";
        }
    }
    else
    {
        mTitle = t;
    }

    this->updateLeftOffset();
}

void UrlField::emptyIconAndTitle()
{
    if (mTitle.isEmpty() && mIcon.isNull()) return;

    this->setTitle("");
    this->setIcon(QPixmap());

    this->updateLeftOffset();
}

void UrlField::urlChanged(QUrl url)
{
    this->setText(url.toString());
    this->setCursorPosition(0);
}

void UrlField::textChange()
{
    WebView *webView = Application::getWindow()->getWebContainer()->getWebView();

    if (this->text().isEmpty() || this->text() != webView->url().toString())
    {
        this->emptyIconAndTitle();
    }
}

void UrlField::returnPress()
{
    if (Application::getWindow()->getSearchView()->hasSelection() != -1)
    {
        Application::getWindow()->getSearchView()->executeRequestFromIndex(Application::getWindow()->getSearchView()->hasSelection());
        return;
    }

    MainWindow *m = Application::getWindow();
    m->getWebContainer()->getWebView()->findText("", QWebPage::HighlightAllOccurrences);
    m->getWebContainer()->setFocus();
    m->getSearchView()->hide();

    this->emptyIconAndTitle();

    this->setIcon(QPixmap(":/favicon.png"));

    if (m->getWebContainer() != NULL)
        m->getWebContainer()->loadUrl(this->text());
}

void UrlField::keyPressEvent(QKeyEvent *e)
{
    QLineEdit::keyPressEvent(e);
}

void UrlField::keyReleaseEvent(QKeyEvent *e)
{
    Application::getWindow()->getSearchView()->show();
    Application::getWindow()->getSearchView()->setup(this->text());

    switch (e->key()) {
    case Qt::Key_Escape:
    {
        WebView *webView = Application::getWindow()->getWebContainer()->getWebView();

        this->setText(webView->url().toString());
        this->setIcon(webView->icon().pixmap(16, 16));
        this->setTitle(webView->title());

        if (!mIcon.isNull() || !mTitle.isEmpty())
        {
            this->clearFocus();
            Application::getWindow()->getWebContainer()->setFocus();
        }

        Application::getWindow()->getWebContainer()->getWebView()->findText("", QWebPage::HighlightAllOccurrences);
        Application::getWindow()->getSearchView()->hide();

        break;
    }
    case Qt::Key_Down:
    {
        this->clearFocus();

        Application::getWindow()->getSearchView()->setFocus();
        Application::getWindow()->getSearchView()->show();
        Application::getWindow()->getSearchView()->selectFirstItem();

        return;
    }
    default:
        break;
    }

    QLineEdit::keyReleaseEvent(e);
}

void UrlField::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();
    rect.setHeight(rect.height() - 1);

    p.translate(0, 1);

    p.setBrush(QColor("#efefef"));
    p.drawRoundedRect(rect, 2, 2);

    p.translate(0, -1);

    QLinearGradient g(0, 0, 0, this->height());
    g.setColorAt(0, QColor("#838383"));
    g.setColorAt(1, QColor("#b9b9b9"));
    p.setBrush(g);
    p.drawRoundedRect(rect, 2, 2);

    rect.setX(1);
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 2);
    p.translate(0, 1);

    p.setBrush(QColor("#c0c0c0"));
    p.drawRoundedRect(rect, 2, 2);

    rect.setHeight(rect.height() - 1);
    p.translate(0, 1);

    g = QLinearGradient(0, 0, 0, this->height());
    g.setColorAt(0, QColor("#dddddd"));
    g.setColorAt(1, QColor("#fbfbfb"));
    p.setBrush(g);
    p.drawRoundedRect(rect, 2, 2);

    /* Progress Bar */
    if (mProgress > 0 && mProgress < 100)
    {
        g = QLinearGradient(0, 0, this->width(), 0);
        g.setColorAt(0, QColor("#a3cae9"));
        g.setColorAt(.9, QColor(0, 0, 0, 0));
        p.setBrush(g);
        p.setOpacity(.75);
        p.drawRoundedRect(rect.x(), rect.y() - 1, this->width()*mProgress/100, rect.height() + 1, 2, 2);
    }

    int xOffset = 10;
    if (!mIcon.isNull())
    {
        p.drawPixmap(xOffset, 3, mIcon.width(), mIcon.height(), mIcon);
        xOffset += 12 + 10;
    }
    if (!mTitle.isEmpty())
    {
        QFontMetrics metrics = this->fontMetrics();

        p.translate(0, 1);
        p.setPen(QColor("#f0f0f0"));
        p.drawText(xOffset, 0, metrics.width(mTitle), this->height() - 4, Qt::AlignVCenter, mTitle);

        p.translate(0, -1);
        p.setPen(QColor("#a3a3a3"));
        p.drawText(xOffset, 0, metrics.width(mTitle), this->height() - 4, Qt::AlignVCenter, mTitle);

        xOffset += metrics.width(mTitle) + 5;
        p.drawText(xOffset, 0, metrics.width(QChar(0x2022)), this->height() - 4, Qt::AlignVCenter, QChar(0x2022));

        xOffset += metrics.width(QChar(0x2022)) + 5;
    }

    QLineEdit::paintEvent(e);
}
