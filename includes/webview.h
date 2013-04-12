#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QtWebKit>
#include <QtWebKitWidgets>

class WebView : public QWebView
{
    Q_OBJECT
public:
    WebView(QWidget *parent = 0);

    int focusedIndexElement();
    void setFocusedElement(int index);

    void takeScreenshot();
    void releaseScreenshot();

protected slots:
    void loadFinish(bool);

protected:
    void wheelEvent(QWheelEvent *);
    void dropEvent(QDropEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void paintEvent(QPaintEvent *);

private:
    QPixmap mWebPixmap;
};

#endif // WEBVIEW_H
