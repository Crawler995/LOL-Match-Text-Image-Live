#include "lolmatchliveapp.h"
#include "components/mainwindow/mainwindow.h"
#include "features/fontloader.h"

#include <QApplication>

LOLMatchLiveApp::LOLMatchLiveApp(QWidget *parent) :
    MainWindow("LOL Match Text&Image Live", parent)
{
    contentArea = new ContentArea(mainWidget);
    initWindowComponent(contentArea);

    QApplication::setFont(fontLoader("/../fonts/BeaufortforLOLJa-Regular.ttf"));
}
