#ifndef LIVEAREA_H
#define LIVEAREA_H

#include "pointinfoarea.h"
#include "../areatitle.h"
#include "matchinfo.h"
#include "../features/live/matchinfoparser.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>

class LiveArea : public QWidget {
    Q_OBJECT
public:
    explicit LiveArea(QWidget *parent = 0);
    ~LiveArea();

    MatchParser *getMatchInfoParser() const;

public slots:
    void startParsingMatchInfo(int matchID);

private:
    QVBoxLayout *layout;

    AreaTitle *title;
    MatchInfo *matchInfo;
    PointInfoArea *pointInfoArea;

    MatchParser *matchParser;
    QThread *parseThread;

    void deletePreParseThread();

private slots:
    void setMatchInfo();
    void setPointInfo();
    void showNetworkError(const QString &error);
};

#endif // LIVEAREA_H
