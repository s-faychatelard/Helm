#include "../../includes/tab.h"

#include "../../includes/application.h"

#include <QtCore/QDebug>

TabItem::TabItem(QWidget *parent) : QAbstractButton(parent)
{
    this->setFixedHeight(25);
}

void TabItem::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);

    QRect rect = this->rect();

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

    p.setPen(Qt::black);
    p.drawText(rect, Qt::AlignCenter, this->text());
}

Tab::Tab(QWidget *parent) : QWidget(parent)
{
    this->setAcceptDrops(false);
    this->setFixedHeight(50);
}

void Tab::clicked()
{
    TabItem *item = (TabItem*)QObject::sender();
    Application::getWindow()->getWebContainer()->switchToTab(mItems.indexOf(item));
}


void Tab::updateWithWebViews(QList<WebView*>* webviews)
{
    mItems.clear();

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

        item->resize(50, 0);

        item->move(xOffset, 12);
        item->show();

        mItems.push_back(item);

        xOffset+=item->width() + 5;
    }
}
