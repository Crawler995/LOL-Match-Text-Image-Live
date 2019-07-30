#include "settingtitle.h"
#include "features/colorboard.h"

SettingTitle::SettingTitle(const QString &title, QWidget *parent) : QLabel(title, parent) {
    setStyleSheet("color: " + ColorBoard::gold4 + ";"
                  "font-size: 16px;");
}
