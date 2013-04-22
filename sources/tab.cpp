#include "../../includes/tab.h"

#include "../../includes/application.h"

#define MAX_WIDTH_TAB 150

TabItem::TabItem(QWidget *parent) : QAbstractButton(parent)
{
    mSelected = false;

    this->setFixedHeight(24);
}

void TabItem::setSelected(bool isSelected)
{
    mSelected = isSelected;

    this->repaint();
}

void TabItem::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);

    QRect rect = this->rect();

    if (mSelected)
        p.setBrush(QColor("#7683c6"));
    else
        p.setBrush(QColor("#c8c8c8"));
    p.drawRoundedRect(rect, 2, 2);

    rect.setX(rect.x() + 1);
    rect.setY(rect.y() + 1);
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);

    QLinearGradient g(0, 0, 0, this->height());
    g.setColorAt(0, QColor("#ffffff"));
    g.setColorAt(1, QColor("#e8e8e8"));
    p.setBrush(g);
    p.drawRoundedRect(rect, 2, 2);

    if (mSelected)
    {
        p.setOpacity(.1);
        p.setBrush(QColor("#7683c6"));
        p.drawRoundedRect(rect, 2, 2);
        p.setOpacity(1);
    }

    rect.setX(rect.x() + 10);
    rect.setWidth(rect.width() - 10);

    if (!this->icon().isNull())
    {
        p.drawPixmap(5, 4, 16, 16, this->icon().pixmap(16, 16));
        rect.setX(16 + 5 + 5);
        rect.setWidth(this->width() - rect.x());
    }
    p.setPen(Qt::black);
    p.drawText(rect, Qt::AlignVCenter, this->text());
}

Tab::Tab(QWidget *parent) : QWidget(parent)
{
    this->setAcceptDrops(false);
    this->setFixedHeight(50);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setAlignment(Qt::AlignCenter);
    hLayout->setSpacing(5);
    hLayout->setMargin(0);
    this->setLayout(hLayout);

    mHome = new TabItem(this);
    connect(mHome, SIGNAL(clicked()), this, SLOT(clicked()));
    mHome->setText("");
    mHome->setIcon(QIcon(QPixmap(":/home.png")));
    mHome->setFixedWidth(26);
    mHome->setSelected();
    this->layout()->addWidget(mHome);
}

void Tab::clicked()
{
    TabItem *item = (TabItem*)QObject::sender();

    mHome->setSelected(false);

    int index = mItems.indexOf(item);
    if (index == -1)
    {
        Application::getWindow()->getWebContainer()->showHome();
        return;
    }

    Application::getWindow()->getWebContainer()->switchToTab(index);
}

TabItem *Tab::getHome()
{
    return mHome;
}

void Tab::select(int index)
{
    for(int i=0; i<mItems.count(); i++)
    {
        mItems.at(i)->setSelected(false);
        if (i == index)
            mItems.at(i)->setSelected();
    }
}

void Tab::updateWithWebViews(QList<WebView*>* webviews)
{
    for(int i=0; i<mItems.count(); i++)
    {
        this->layout()->removeWidget(mItems.at(i));
        mItems.at(i)->hide();
    }

    mItems.clear();

    /*if (webviews->count() <= 1)
        this->hide();
    else
        this->show();*/
    this->show();

    int xOffset=10;
    for (int i=0; i<webviews->count(); i++)
    {
        TabItem *item = new TabItem(this);

        connect(item, SIGNAL(clicked()), this, SLOT(clicked()));

        item->setText(webviews->at(i)->title());

        if (item->text().isEmpty())
        {
            item->setText("No title");
        }

        if (webviews->at(i)->icon().isNull())
            item->setIcon(QIcon(QPixmap(":/favicon.png")));
        else
            item->setIcon(webviews->at(i)->icon());

#define MAX_TITLE_LENGTH 30
        if (item->text().length() > MAX_TITLE_LENGTH)
        {
            if (item->text().contains("-"))
            {
                item->setText(item->text().left(item->text().indexOf("-")));
            }
            else if (item->text().contains("|"))
            {
                item->setText(item->text().left(item->text().indexOf("|")));
            }

            if (item->text().length() > MAX_TITLE_LENGTH)
            {
                item->setText(item->text().left(MAX_TITLE_LENGTH - 3) + "...");
            }
        }
        QFontMetrics metrics = item->fontMetrics();
        int width = metrics.width(item->text());

        item->setFixedWidth(width + 32);

        this->layout()->addWidget(item);

        mItems.push_back(item);

        xOffset+=item->width() + 5;
    }
}
