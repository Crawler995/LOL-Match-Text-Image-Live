#ifndef MATCHINFOPARSER_H
#define MATCHINFOPARSER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QString>
#include <QRegExp>
#include <QVector>
#include <QStringList>
#include <QPixmap>
#include <QTimer>

struct PointInfoContent {
    QString time;
    QString text;
    QString image;
};

class MatchParser : public QObject {
    Q_OBJECT
public:
    MatchParser(int matchID);
    ~MatchParser();


    void startParsingMatchAndPointInfo();

    int getTeamAScore() const;
    int getTeamBScore() const;
    QString getTeamAName() const;
    QString getTeamBName() const;
    QString getMatchStatus() const;
    QString getMatchTime() const;

    QUrl getLiveUrl() const;
    QVector<PointInfoContent *> getPointInfoContents() const;

public slots:
    void startParsingMatchInfo();

private:
    QUrl matchInfoUrl;
    QNetworkAccessManager *matchInfoManager, *pointInfoManager;

    int teamAScore;
    int teamBScore;
    QString teamAName;
    QString teamBName;
    QString matchStatus;
    QString matchTime;

    QUrl liveUrl;
    QVector<PointInfoContent*> pointInfoContents;
    int prePointInfoContentNum;
    bool isFirstGetPointInfoFromDuoWan;

    QTimer *timer;

    QString captureText(QRegExp regx, const QString &totalMatchInfoString);
    QString removeSomeHTMLDom(QString str);
    QStringList findAll(QRegExp regx, const QString &str);

    QString getRawPointInfoCodeName(const QString &totalPointInfoString);
    QString convertAnyCodeToUTF8(QByteArray &bytes, const QString &codeName);
    void addNewPointInfoContentNotFromDuoWan(const QString &totalPointInfoString);
    void addNewPointInfoContentFromDuoWan(const QJsonObject &totalPointInfoJsonObj);
    void changeLiveUrlFromDuoWan(const QByteArray &pointInfoByteArray);

    QString formatPointInfoTimeNotFromDuoWan(const QString &pointInfoTime);
    QString formatPointInfoTimeFromDuoWan(const QString &pointInfoTime);
    QString captureTextFromPointInfoTextAndImage(const QString &pointInfoTextAndImage);
    QString captureImageUrlFromPointInfoTextAndImage(const QString &pointInfoTextAndImage);
    int getActIDFromDuoWan(const QString &totalPointInfoString);

private slots:
    void startParsingPointInfo();
    void parsingMatchInfoFinished(QNetworkReply *matchInfoReply);
    void parsingPointInfoFinished(QNetworkReply *pointInfoReply);

signals:
    void canGetMatchInfo();
    void canGetPointInfo();
    void getInfoNetworkError(const QString &error);
};

#endif // MATCHINFOPARSER_H
