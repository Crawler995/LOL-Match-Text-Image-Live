#include "lolmatchliveapp.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LOLMatchLiveApp w;
    w.show();

    return a.exec();
}
