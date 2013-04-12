#include "../../includes/centralwidget.h"

#include <QtCore/QDebug>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    this->setAcceptDrops(false);
}
