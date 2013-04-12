#include "../../includes/webviewcookie.h"

#include "../../includes/application.h"

#include <QSettings>

static const unsigned int JAR_VERSION = 23;

QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{
    stream << JAR_VERSION;
    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();

    quint32 version;
    stream >> version;

    if (version != JAR_VERSION)
        return stream;

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i)
    {
        QByteArray value;
        stream >> value;

        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0)
        {
            qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
        }

        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));

        if (stream.atEnd())
            break;
    }
    return stream;
}
QT_END_NAMESPACE

WebViewCookie::WebViewCookie(QObject *parent) : QNetworkCookieJar(parent)
{
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    QSettings cookieSettings(Application::dataLocation() + "cookies.db", QSettings::IniFormat);
    this->setAllCookies(qvariant_cast<QList<QNetworkCookie> >(cookieSettings.value("cookies")));
}

WebViewCookie::~WebViewCookie()
{
    QSettings cookieSettings(Application::dataLocation() + "cookies.db", QSettings::IniFormat);

    QList<QNetworkCookie> cookies = this->allCookies();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        if (cookies.at(i).isSessionCookie())
            cookies.removeAt(i);
    }

    cookieSettings.setValue("cookies", QVariant::fromValue<QList<QNetworkCookie> >(cookies));
}
