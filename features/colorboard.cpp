#include "colorboard.h"

#include <QString>
#include <QColor>

QString ColorBoard::black1 = "#0a0a0c";
QString ColorBoard::black2 = "#131315";

QString ColorBoard::gold1 = "#342b20";
QString ColorBoard::gold2 = "#7d6033";
QString ColorBoard::gold3 = "#8a6c3c";
QString ColorBoard::gold4 = "#b59758";
QString ColorBoard::gold5 = "#c4b998";


QString ColorBoard::convertHexColorToRGBA(const QString &color, double alpha) {
    QColor c(color);
    return QString("rgba(%1, %2, %3, %4)").arg(c.red()).arg(c.green()).arg(c.blue()).arg(alpha);
}
