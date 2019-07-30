#ifndef MATCHINFO_H
#define MATCHINFO_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMovie>

class MatchInfo : public QWidget {
    Q_OBJECT
public:
    explicit MatchInfo(QWidget *parent = 0);

    void setTeamAName(const QString &name);
    void setTeamBName(const QString &name);
    void setTeamAScore(int score);
    void setTeamBScore(int score);
    void setMatchTime(const QString &time);
    void setMatchStatus(const QString &status);

    void loadParsingGif();

public slots:
    void clearParsingGif();

private:
    QGridLayout *layout;
    QLabel *teamANameLabel, *matchTimeLabel, *scoreLabel, *matchStatusLabel, *teamBNameLabel;
    QString teamAName, teamBName;
    int teamAScore, teamBScore;
    QString matchTime, matchStatus;
    QMovie *parsingGif;

    QLabel* createAlignCenterLabel(const QString &text, QWidget *parent = 0);  
};

#endif // MATCHINFO_H
