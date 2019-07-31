#include "utils.h"

#include <QDebug>
#include <QDir>
#include <QDateTime>

void LDebug(const QString debugInfo, const QString file, int line) {
    qInfo() << "\n-----\n"
            << " DEBUG INFO:\n"
            << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz") << "\n"
            << QDir::fromNativeSeparators(file) << "line" << line << "\n"
            << debugInfo << "\n"
            << "-----";
}
