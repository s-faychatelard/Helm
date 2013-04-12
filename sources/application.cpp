#include <QtCore/QProcess>
#include <QtCore/QEvent>
#include <QtCore/QDir>

#include <QtCore/QStandardPaths>

#include "../../includes/application.h"

static MainWindow *w = NULL;

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    this->setOrganizationName(QLatin1String("Dviance"));
    this->setApplicationName(QLatin1String("Helm"));
    this->setApplicationVersion(QLatin1String("0.1"));

    QFont f = this->font();
    f.setFamily("Arial");
    this->setFont(f);

    QDir dir;
    dir.mkpath(Application::dataLocation() + "Icon/");
    dir.mkpath(Application::dataLocation() + "Local/");
    dir.mkpath(Application::dataLocation() + "Offline/Data/");
    dir.mkpath(Application::dataLocation() + "Offline/Cache/");

    QDesktopWidget d;
    w = new MainWindow();
    w->resize(d.availableGeometry().width() / 3*2, d.availableGeometry().height()/3*2);
    w->move(d.availableGeometry().width()/2 - w->width()/2, d.availableGeometry().height()/2 - w->height()/2);
    w->show();
    w->raise();
}

Application::~Application()
{
    w->close();
    delete w;
    w = NULL;
}

MainWindow *Application::getWindow()
{
    return w;
}

void Application::restart()
{
    QProcess	mProcess;
    QString str = "";
#if defined(Q_OS_WIN32)
    str = QString("\"%1\"").arg(QApplication::arguments().at(0));
#elif defined(Q_OS_DARWIN)
    str = QString(QApplication::arguments().at(0));
    str.replace(" ","\\ ");
#else
    str = QString(QApplication::arguments().at(0));
#endif
    mProcess.startDetached(str);
    QApplication::quit();
}

QString Application::dataLocation()
{
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/";
}

bool Application::event(QEvent *e)
{
    if (e->type() == QEvent::FileOpen)
    {
        QFileOpenEvent* file = static_cast<QFileOpenEvent*>(e);
        QString url = file->url().toString();
        if (url.startsWith("file:"))
        {
            url.remove(0, QString("file:").length());
        }
        w->getWebContainer()->newTab(url);
        qDebug() << url;

        return true;
    }
    else
    {
        return QApplication::event(e);
    }
}
