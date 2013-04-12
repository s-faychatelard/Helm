#ifndef WEBVIEWCOOKIE_H
#define WEBVIEWCOOKIE_H

#include <QtWebKit>
#include <QtWebKitWidgets>

class WebViewCookie : public QNetworkCookieJar
{
    Q_OBJECT
public:
    WebViewCookie(QObject *parent = 0);
    ~WebViewCookie();
};

#endif // WEBVIEWCOOKIE_H
