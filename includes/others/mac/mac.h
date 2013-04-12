#ifndef MAC_H
#define MAC_H

#include <QtCore>
#include <QtWidgets>

class Mac : public QObject
{
	Q_OBJECT
public:
    Mac(QWidget* w = 0);
    void enterFullscreenMode();
    void registerAsDefaultBrowser();

private:
    QWidget *mParent;
};

#endif
