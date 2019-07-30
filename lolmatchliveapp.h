#ifndef LOLMATCHLIVEAPP_H
#define LOLMATCHLIVEAPP_H

#include "components/mainwindow/mainwindow.h"
#include "components/mainwindow/contentarea.h"
#include <QWidget>

class LOLMatchLiveApp : public MainWindow {
    Q_OBJECT
public:
    explicit LOLMatchLiveApp(QWidget *parent = 0);

private:
    ContentArea *contentArea;
};

#endif // LOLMATCHLIVEAPP_H
