#ifndef WINDOWS_H
#define WINDOWS_H

#include <QtCore>
#include <QtWidgets>

class Windows : public QObject
{
	Q_OBJECT
public:
    Windows(QWidget* w = 0);
};

#endif
