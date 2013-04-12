#include <QtGui/QApplication>

#include "../includes/application.h"
#include "../includes/mainwindow.h"

int main(int argc, char *argv[])
{
    int result = -1;
    Application a(argc, argv);
    result =  a.exec();
    QApplication::exit(result);
    return result;
}
