#ifndef URLFIELD_H
#define URLFIELD_H

#include <QLineEdit>

#include <QtCore>

class UrlField : public QLineEdit
{
    Q_OBJECT
public:
    UrlField(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

protected:
    void paintEvent(QPaintEvent *);

protected slots:
    void editingEnd();
    void textChange();
    void returnPress();

public slots:
    void urlChanged(QUrl);
    void setProgress(int);
    void setIcon(QPixmap);
    void setTitle(QString);
    void setText(const QString &);

private:
    QPixmap mIcon;
    QString mTitle;

    int mProgress;

    void updateLeftOffset();
    void emptyIconAndTitle();
};

#endif // URLFIELD_H
