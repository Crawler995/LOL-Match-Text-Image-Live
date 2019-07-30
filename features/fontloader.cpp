#include "fontloader.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QString>
#include <QStringList>

QFont fontLoader(const QString &fontFilePath) {
    int index = QFontDatabase::addApplicationFont(QApplication::applicationDirPath() + fontFilePath);
    if (index != -1) {
        QStringList strList(QFontDatabase::applicationFontFamilies(index));
        if (strList.count() > 0) {
            QFont font(strList.at(0));
            return font;
        }
    }
    else {
        return QFont("Microsoft Yahei");
    }
}
