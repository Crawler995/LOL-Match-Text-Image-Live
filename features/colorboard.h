#ifndef COLORBOARD_H
#define COLORBOARD_H

#include <QString>

class ColorBoard {
public:
    static QString black1;
    static QString black2;

    static QString gold1;
    static QString gold2;
    static QString gold3;
    static QString gold4;
    static QString gold5;

    static QString convertHexColorToRGBA(const QString &color, double alpha);
};

#endif // COLORBOARD_H
