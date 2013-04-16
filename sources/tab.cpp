#include "../../includes/tab.h"

#include "../../includes/application.h"

#include <QtCore/QDebug>

#define MAX_WIDTH_TAB 150

TabItem::TabItem(QWidget *parent) : QAbstractButton(parent)
{
    this->setFixedHeight(25);
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

    rect.setX(rect.x() + 10);
    rect.setWidth(rect.width() - 10);

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
    hLayout->setMargin(10);
    this->setLayout(hLayout);
}

void Tab::clicked()
{
    TabItem *item = (TabItem*)QObject::sender();
    Application::getWindow()->getWebContainer()->switchToTab(mItems.indexOf(item));
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

    if (webviews->count() <= 1)
        this->hide();
    else
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

        QFontMetrics metrics = item->fontMetrics();
        int width = metrics.width(item->text());

        if (width > MAX_WIDTH_TAB)
            width = MAX_WIDTH_TAB;

        item->setFixedWidth(width + 25);

        this->layout()->addWidget(item);

        mItems.push_back(item);

        xOffset+=item->width() + 5;
    }
}
