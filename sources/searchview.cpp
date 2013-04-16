#include "../../includes/searchview.h"

#include "../../includes/application.h"
#include "../../includes/scrollbar.h"

#include <QtXml/QtXml>

SearchViewTitle::SearchViewTitle(QString t, QWidget *p) : QAbstractButton(p)
{
    this->setFixedHeight(22);
    this->resize(this->parentWidget()->width(), 0);

    mTitle = t;
}

void SearchViewTitle::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QFont f = p.font();
    f.setPixelSize(13);
    p.setFont(f);

    p.translate(0, 1);
    p.setPen(QColor("#ffffff"));
    p.drawText(12, 0, this->width(), this->height(), Qt::AlignVCenter, mTitle);

    p.translate(0, -1);
    p.setPen(QColor("#bfbfbf"));
    p.drawText(12, 0, this->width(), this->height(), Qt::AlignVCenter, mTitle);
}

SearchViewSeparator::SearchViewSeparator(QWidget *p) : QWidget(p)
{
    this->setFixedHeight(2);
    this->resize(this->parentWidget()->width(), 0);
}

void SearchViewSeparator::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QColor("#bfbfbf"));
    p.drawLine(1, 0, this->width() - 2, 0);

    p.setPen(QColor("#ffffff"));
    p.drawLine(1, 1, this->width() - 2, 1);
}

SearchViewItem::SearchViewItem(QString t, QWidget *p) : QAbstractButton(p)
{
    this->setFixedHeight(22);
    this->resize(this->parentWidget()->width(), 0);

    connect(this, SIGNAL(clicked()), Application::getWindow()->getSearchView(), SLOT(selectItem()));

    mHover = false;
    mText = t;
}

QString SearchViewItem::getText()
{
    return mText;
}

void SearchViewItem::enterEvent(QEvent *)
{
    SearchView * s = Application::getWindow()->getSearchView();
    s->select(this);
}

void SearchViewItem::leaveEvent(QEvent *)
{
    this->setHover(false);
}

void SearchViewItem::setHover(bool hover)
{
    mHover = hover;

    this->repaint();
}

void SearchViewItem::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QFont f = p.font();
    f.setPixelSize(13);
    p.setFont(f);

    if (mHover)
    {
        p.setPen(Qt::NoPen);

        QLinearGradient g(0, 0, 0, this->height());
        g.setColorAt(0, QColor("#476bee"));
        g.setColorAt(1, QColor("#083deb"));
        p.setBrush(g);
        p.drawRoundedRect(this->rect(), 2, 2);

        p.translate(0, 1);
        p.setPen(QColor("#5463f8"));
        p.drawText(24, 0, this->width(), this->height(), Qt::AlignVCenter, mText);

        p.translate(0, -1);
        p.setPen(QColor("#ffffff"));
        p.drawText(24, 0, this->width(), this->height(), Qt::AlignVCenter, mText);
    }
    else
    {
        p.translate(0, 1);
        p.setPen(QColor("#ffffff"));
        p.drawText(24, 0, this->width(), this->height(), Qt::AlignVCenter, mText);

        p.translate(0, -1);
        p.setPen(QColor("#525252"));
        p.drawText(24, 0, this->width(), this->height(), Qt::AlignVCenter, mText);
    }
}

SearchView::SearchView(QWidget *parent) : QWidget(parent)
{
    mItemIndex = -1;

    mScrollArea = new QScrollArea(this);
    mScrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    mWidget = new QWidget(mScrollArea);
    mWidget->setStyleSheet("QWidget { background: transparent; } ");
    mScrollArea->setWidget(mWidget);

    ScrollBar *scrollBar = new ScrollBar();
    mScrollArea->setVerticalScrollBar(scrollBar);

    QGraphicsDropShadowEffect * e = new QGraphicsDropShadowEffect(this);
    e->setOffset(0, 10);
    e->setBlurRadius(30);
    e->setColor(QColor(0, 0, 0, .2 * 255));

    this->setGraphicsEffect(e);

    mServices.insert("Google", "https://google.com/search?q=%0");
    mServices.insert("Facebook", "https://www.facebook.com/search/results.php?q=%0");
    mServices.insert("Twitter", "https://twitter.com/search?q=%0");
    mServices.insert("Wikipedia", "https://wikipedia.org/w/index.php?search=%0");
    mServices.insert("Yelp", "http://www.yelp.com/search?find_desc=%0");

    connect(&mManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(requestEnded(QNetworkReply *)));
}

int SearchView::hasSelection()
{
    return mItemIndex;
}

QWidget *SearchView::getWidget()
{
    return mWidget;
}

void SearchView::setup(QString word)
{
    mWord = word;

    QNetworkRequest request;
    request.setUrl(QUrl("http://suggestqueries.google.com/complete/search?output=toolbar&hl=fr&q=" + mWord));
    mManager.get(request);
}

void SearchView::requestEnded(QNetworkReply *e)
{
    for(int i=0; i<mWidgets.count(); i++)
        mWidgets.at(i)->hide();
    mWidgets.clear();

    QDomDocument xml;
    if (xml.setContent(e->readAll()))
    {
        QDomElement root = xml.documentElement();
        QDomNodeList topLevel = root.childNodes();

        /* Google search */
        if (topLevel.count() > 0)
        {
            SearchViewTitle *titleGoogleSearch = new SearchViewTitle("Google Search", mWidget);
            mWidgets.push_back(titleGoogleSearch);

            mGoogleSearch.clear();
        }

        for(int i=0; i<topLevel.count(); i++)
        {
            QDomElement suggestion = topLevel.at(i).toElement();
            QDomNodeList nodes = suggestion.childNodes();
            for(int j=0; j<nodes.count(); j++)
            {
                QDomElement s = nodes.at(j).toElement();
                if (s.tagName() == "suggestion")
                {
                    SearchViewItem *it = new SearchViewItem(s.attribute("data"), mWidget);
                    mWidgets.push_back(it);
                    mGoogleSearch.push_back(s.attribute("data"));
                }
            }
        }
    }

    if (mGoogleSearch.count() > 0)
    {
        SearchViewSeparator *sepGoogleSearch = new SearchViewSeparator(mWidget);
        mWidgets.push_back(sepGoogleSearch);
    }

    /* Service */
    SearchViewTitle *titleService = new SearchViewTitle("Services", mWidget);
    mWidgets.push_back(titleService);

    for(int i=0; i<mServices.keys().count(); i++)
    {
        QString s = "\"";
        s += mWord;
        s += "\" on ";
        s += mServices.keys().at(i);

        SearchViewItem *it = new SearchViewItem(s, mWidget);
        mWidgets.push_back(it);
    }

    if (mWord.isEmpty())
    {
        this->organize();
        return;
    }

    if (Application::getWindow()->getWebContainer()->getWebView()->url().isLocalFile())
    {
        SearchViewSeparator *sepService = new SearchViewSeparator(mWidget);
        mWidgets.push_back(sepService);

        /* On this page */
        SearchViewTitle *titleOnThisPage = new SearchViewTitle("On this page", mWidget);
        mWidgets.push_back(titleOnThisPage);

        QString oc = mWord;
        oc += " (";
        oc += QString::number(searchOccurrences());
        oc += " results)";
        SearchViewItem *it = new SearchViewItem(oc, mWidget);
        mWidgets.push_back(it);
    }

    /*Application::getWindow()->getWebContainer()->getWebView()->findText("", QWebPage::HighlightAllOccurrences);
    Application::getWindow()->getWebContainer()->getWebView()->findText(mWord, QWebPage::HighlightAllOccurrences);
    for(int i=0; i<mOnThisPage.count(); i++)
    {
        SearchViewItem *it = new SearchViewItem(mOnThisPage.at(i), mWidget);
        mWidgets.push_back(it);
    }*/
    this->organize();
}

int SearchView::searchOccurrences()
{
    int count=0;
    Application::getWindow()->getWebContainer()->getWebView()->findText("");
    while(Application::getWindow()->getWebContainer()->getWebView()->findText(mWord) == true)
    {
        count++;
    }
    Application::getWindow()->getWebContainer()->getWebView()->findText("");
    return count;
}

void SearchView::selectFirstItem()
{
    if (mItemIndex != -1)
    {
        SearchViewItem *it = dynamic_cast<SearchViewItem*>(mWidgets.at(mItemIndex));
        if (it != NULL)
            it->setHover(false);
    }

    for(int i=0; i<mWidgets.count(); i++)
    {
        SearchViewItem *it = dynamic_cast<SearchViewItem*>(mWidgets.at(i));
        if (it != NULL)
        {
            mItemIndex = i;
            it->setHover();
            break;
        }
    }
}

void SearchView::select(QWidget *w)
{
    if (mItemIndex != -1 && mItemIndex < mWidgets.count())
    {
        SearchViewItem *it = dynamic_cast<SearchViewItem*>(mWidgets.at(mItemIndex));
        if (it != NULL)
            it->setHover(false);
    }

    int index = mWidgets.indexOf(w);

    if (index == -1) return;

    SearchViewItem *it = dynamic_cast<SearchViewItem*>(mWidgets.at(index));
    if (it != NULL)
        it->setHover();

    mItemIndex = index;
}

void SearchView::selectItem()
{
    SearchViewItem *it = (SearchViewItem*)QObject::sender();

    int index = mWidgets.indexOf(it);

    this->executeRequestFromIndex(index);
}

void SearchView::executeRequestFromIndex(int index)
{
    SearchViewItem *it = (SearchViewItem*)mWidgets.at(index);

    if (index <= mGoogleSearch.count())
    {
        QString url = mServices.value("Google").arg(it->getText());
        Application::getWindow()->getWebContainer()->getWebView()->load(QUrl(url));

        this->hide();
    }
    else if (index <= mGoogleSearch.count() + mServices.keys().count() + 2)
    {
        QRegExp regExp("\".*\" on (.*)");
        if (regExp.exactMatch(it->getText()))
        {
            QString url = mServices.value(regExp.capturedTexts().at(1)).arg(mWord);
            Application::getWindow()->getWebContainer()->getWebView()->load(QUrl(url));

            this->hide();
        }
    }
    else
    {
        Application::getWindow()->getWebContainer()->getWebView()->findText("", QWebPage::HighlightAllOccurrences);
        Application::getWindow()->getWebContainer()->getWebView()->findText(mWord, QWebPage::HighlightAllOccurrences);

        this->hide();
    }
}

void SearchView::organize()
{
    int yOffset=5;
    for(int i=0; i<mWidgets.count(); i++)
    {
        mWidgets.at(i)->show();
        mWidgets.at(i)->move(0, yOffset);
        mWidgets.at(i)->resize(mScrollArea->width(), 0);

        yOffset+=mWidgets.at(i)->height() + 5;
    }
    mWidget->resize(mWidget->width(), yOffset);

    if (yOffset > 500)
        yOffset = 500;

    mScrollArea->resize(mScrollArea->width(), yOffset);

    this->resize(this->width(), yOffset);
}

void SearchView::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Up:
    {
        if (mItemIndex != -1)
            ((SearchViewItem*)mWidgets.at(mItemIndex))->setHover(false);

        for(int i=mItemIndex - 1; i>=0; i--)
        {
            SearchViewItem *it = dynamic_cast<SearchViewItem*>(mWidgets.at(i));

            if (it != NULL)
            {
                mItemIndex = i;

                it->setHover();

                return;
            }
        }

        mItemIndex = -1;
        Application::getWindow()->getTopBar()->getUrlField()->setFocus();

        break;
    }
    case Qt::Key_Down:
    {
        if (mItemIndex != -1)
            ((SearchViewItem*)mWidgets.at(mItemIndex))->setHover(false);

        for(int i=mItemIndex + 1; i<mWidgets.count(); i++)
        {
            SearchViewItem *it = dynamic_cast<SearchViewItem*>(mWidgets.at(i));

            if (it != NULL)
            {
                mItemIndex = i;

                it->setHover();

                return;
            }
        }

        if (mItemIndex != -1)
            ((SearchViewItem*)mWidgets.at(mItemIndex))->setHover();

        break;
    }
    default:
        Application::getWindow()->getTopBar()->getUrlField()->keyPressEvent(e);
        Application::getWindow()->getTopBar()->getUrlField()->keyReleaseEvent(e);

        break;
    }
    QWidget::keyPressEvent(e);
}

void SearchView::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    mWidget->resize(this->width(), mWidget->height());
    mScrollArea->resize(this->width(), this->height());

    this->organize();
}

void SearchView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();

    p.setPen(QColor("#686868"));
    p.setBrush(QColor(255, 255, 255, 230));
    p.drawRoundedRect(rect, 4, 4);

    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(QPen(QColor("#ffffff"), 1));
    p.setBrush(Qt::transparent);
    p.drawRoundedRect(rect.x() + 1, rect.y() + 1, rect.width() - 3, rect.height() - 3, 4, 4);
}

