#include "matchinfo.h"
#include "features/colorboard.h"

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QHBoxLayout>

MatchInfo::MatchInfo(QWidget *parent) : QWidget(parent) {
    teamAScore = teamBScore = 0;
    teamAName = "TEAM A";
    teamBName = "TEAM B";
    matchTime = "1970-01-01 00:00:00";
    matchStatus = "未开始";

    layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    teamANameLabel = createAlignCenterLabel(teamAName, this);
    matchTimeLabel = createAlignCenterLabel(matchTime, this);
    scoreLabel = createAlignCenterLabel(QString("%1 : %2").arg(teamAScore).arg(teamBScore), this);
    matchStatusLabel = createAlignCenterLabel(matchStatus, this);
    teamBNameLabel = createAlignCenterLabel(teamBName, this);

    layout->addWidget(teamANameLabel, 0, 0, 1, 2);
    layout->addWidget(matchTimeLabel, 1, 0, 1, 3);
    layout->addWidget(scoreLabel, 0, 2, 1, 2);
    layout->addWidget(matchStatusLabel, 1, 3, 1, 3);
    layout->addWidget(teamBNameLabel, 0, 4, 1, 2);

    setLayout(layout);

    setStyleSheet("color: " + ColorBoard::gold4 + ";"
                  "background-color: " + ColorBoard::convertHexColorToRGBA(ColorBoard::black2, 0.4) + ";");

    scoreLabel->setStyleSheet("border-left: 1px solid " + ColorBoard::gold1 + ";"
                              "border-right: 1px solid " + ColorBoard::gold1 + ";");
    teamANameLabel->setStyleSheet("QLabel{padding-top: 10px; padding-bottom: 10px;}");
    teamBNameLabel->setStyleSheet("QLabel{padding-top: 10px; padding-bottom: 10px;}");
    matchTimeLabel->setStyleSheet("QLabel{padding-top: 4px; padding-bottom: 4px; font-size: 14px;"
                                  "border-right: 1px solid " + ColorBoard::gold1 + ";"
                                  "border-top: 1px solid " + ColorBoard::gold1 + ";}");
    matchStatusLabel->setStyleSheet("QLabel{padding-top: 4px; padding-bottom: 4px; font-size: 14px;"
                                    "border-top: 1px solid " + ColorBoard::gold1 + ";}");
}

QLabel *MatchInfo::createAlignCenterLabel(const QString &text, QWidget *parent) {
    QLabel *res = new QLabel(text, parent);
    res->setAlignment(Qt::AlignCenter);
//    res->setStyleSheet("QLabel{padding-top: 10px; padding-bottom: 10px;}");
    return res;
}

void MatchInfo::setTeamAName(const QString &name) {
    teamAName = name;
    teamANameLabel->setText(name);
}

void MatchInfo::setTeamBName(const QString &name) {
    teamBName = name;
    teamBNameLabel->setText(name);
}

void MatchInfo::setTeamAScore(int score) {
    teamAScore = score;
}

void MatchInfo::setTeamBScore(int score) {
    teamBScore = score;
}

void MatchInfo::setMatchTime(const QString &time) {
    matchTime = time;
}

void MatchInfo::setMatchStatus(const QString &status) {
    matchStatus = status;
}

void MatchInfo::loadParsingGif() {
    parsingGif = new QMovie(":/icons/images/icons/spinner-2.gif");
    parsingGif->setScaledSize(QSize(40, 40));
    scoreLabel->setMovie(parsingGif);;
    parsingGif->start();
}

void MatchInfo::clearParsingGif() {
    if(parsingGif) {
        parsingGif->stop();
        delete parsingGif;
        parsingGif = nullptr;
    }

    matchTimeLabel->setText(matchTime);
    matchStatusLabel->setText(matchStatus);
    scoreLabel->setText(QString("%1 : %2").arg(teamAScore).arg(teamBScore));
}


