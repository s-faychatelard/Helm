#ifndef WEBCONTAINER_H
#define WEBCONTAINER_H

#include <QtWidgets>

#include "home.h"
#include "tab.h"
#include "webview.h"

#define MARGIN 50
#define VISIBLE_MARGIN 80
#define LEFT_OFFSET VISIBLE_MARGIN + 20
#define ANIMATION_DURATION 160
#define DEFAULT_PAGE "about:blank"

class WebContainer : public QWidget
{
    Q_OBJECT
public:
    WebContainer(QWidget *parent = 0);
    ~WebContainer();

    void loadUrl(QString);

    WebView* getWebView();

    void showHome();
    void showSwitcher();
    void hideSwitcher();
    void switchToNextTab();
    void switchToPreviousTab();
    void switchToTab(int index);

protected:
    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void newTab(QString url = DEFAULT_PAGE);
    void removeTab();

protected slots:
    void iconChange();
    void loadFinish(bool);
    void urlChange(QUrl url);
    void releaseScreenshots();

    void newTabAnimation();
    void newTabAnimation2();

private:
    QList<WebView*> *mWebViews;
    QString mUrlToLoad;

    Home *mHome;
    Tab *mTab;

    int mCurrentWebView;
    int mLastFocusedIndexElement;

    int mNewTabIndex; // Only used for new tab animation

    void hideHome();
    void showTab(int index);
    void disconnectCurrentWebView();
    void switchAnimated(int previousTab, int newTab);
};

#endif // WEBCONTAINER_H
