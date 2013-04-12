#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QtWidgets>
#include <QtNetwork/QNetworkAccessManager>

class SearchViewTitle : public QAbstractButton
{
    Q_OBJECT
public:
    SearchViewTitle(QString, QWidget *p = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    QString mTitle;
};

class SearchViewSeparator : public QWidget
{
    Q_OBJECT
public:
    SearchViewSeparator(QWidget * = 0);

protected:
    void paintEvent(QPaintEvent *);
};

class SearchViewItem : public QAbstractButton
{
    Q_OBJECT
public:
    SearchViewItem(QString, QWidget *p = 0);
    void setHover(bool = true);
    QString getText();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    bool mHover;
    QString mText;
};

class SearchView : public QWidget
{
    Q_OBJECT
public:
    SearchView(QWidget *parent = 0);

    void selectFirstItem();
    void select(QWidget*);
    void setup(QString);
    QWidget *getWidget();

    int hasSelection();
    void executeRequestFromIndex(int);

protected:
    void keyPressEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

protected slots:
    void selectItem();
    void requestEnded(QNetworkReply *);

private:
    QWidget *mWidget;
    QScrollArea *mScrollArea;

    QString mWord;

    QNetworkAccessManager mManager;

    QList<QString> mGoogleSearch;

    QHash<QString, QString> mServices;
    QList<QWidget*> mWidgets;

    int mItemIndex;

    void organize();
    int searchOccurrences();
};

#endif // SEARCHVIEW_H
