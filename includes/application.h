#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#include "mainwindow.h"

class Application : public QApplication
{
	Q_OBJECT
public:
    Application(int &argc, char **argv);
    ~Application();

    static void restart();
    static QString dataLocation();

    static MainWindow *getWindow();

protected:
    bool event(QEvent *);
};

#endif
