#include "matchinfoparser.h"
#include "../../features/utils.h"

#include <QRegExp>
#include <QTimer>
#include <QThread>
#include <QEventLoop>
#include <QPixmap>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MatchParser::MatchParser(int matchID) {
    matchInfoUrl = QUrl(QString("https://lpl.qq.com/web201612/data/LOL_MATCH_DETAIL_%1.js?r=%2")
                   .arg(matchID)
                   .arg(0.1908848324312169));
    prePointInfoContentNum = 0;
    matchInfoManager = pointInfoManager = nullptr;
    timer = nullptr;
    isFirstGetPointInfoFromDuoWan = true;
}

MatchParser::~MatchParser() {
    if(matchInfoManager) {
        delete matchInfoManager;
    }
    if(pointInfoManager) {
        delete pointInfoManager;
    }

    for(auto i = pointInfoContents.constBegin(); i != pointInfoContents.constEnd(); i++) {
        delete *i;
    }
    pointInfoContents.clear();

    if(timer) {
        timer->stop();
        timer->deleteLater();
    }
}

void MatchParser::startParsingMatchAndPointInfo() {
    QTimer::singleShot(0, this, SLOT(startParsingMatchInfo()));

    timer = new QTimer();
    timer->setInterval(60000);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(startParsingMatchInfo()));
    timer->start();
}

void MatchParser::parsingMatchInfoFinished(QNetworkReply *matchInfoReply) {
    if(matchInfoReply->error() != QNetworkReply::NoError) {
        LDebug(QString("parsing match info network error: %1").arg(matchInfoReply->errorString()), __FILE__, __LINE__);
        emit getInfoNetworkError(matchInfoReply->errorString());
        return;
    }
    LDebug("parsing match info finished", __FILE__, __LINE__);
    QString totalMatchInfoString =  QString::fromUtf8(matchInfoReply->readAll());
    delete matchInfoReply;

    QRegExp teamAScoreRegx("\"ScoreA\":\"(\\d+)\"");
    teamAScore = captureText(teamAScoreRegx, totalMatchInfoString).toInt();
    QRegExp teamBScoreRegx("\"ScoreB\":\"(\\d+)\"");
    teamBScore = captureText(teamBScoreRegx, totalMatchInfoString).toInt();
    QRegExp teamANameRegx("\"TeamNameA\":\"(\\S+)\"");
    teamAName = captureText(teamANameRegx, totalMatchInfoString);
    QRegExp teamBNameRegx("\"TeamNameB\":\"(\\S+)\"");
    teamBName = captureText(teamBNameRegx, totalMatchInfoString);
    QRegExp matchStatusRegx("\"MatchStatus\":\"(\\S+)\"");
    QStringList matchStatusStr;
    matchStatusStr << "未开始" << "即将开始或正在进行" << "已结束";
    matchStatus = matchStatusStr.at(captureText(matchStatusRegx, totalMatchInfoString).toInt() - 1);
    QRegExp matchTimeRegx("\"MatchTime\":\"(.*)\"");
    matchTime = captureText(matchTimeRegx, totalMatchInfoString);

    if(isFirstGetPointInfoFromDuoWan) {
        QRegExp liveUrlRegx("\"sExt2Src\":\"(.*)\"");
        liveUrl = QUrl(captureText(liveUrlRegx, totalMatchInfoString).replace("\\", ""));
    }

    startParsingPointInfo();

    emit canGetMatchInfo();
}

void MatchParser::parsingPointInfoFinished(QNetworkReply *pointInfoReply) {
    if(pointInfoReply->error() != QNetworkReply::NoError) {
        LDebug(QString("parsing point info network error: %1").arg(pointInfoReply->errorString()), __FILE__, __LINE__);
        emit getInfoNetworkError(pointInfoReply->errorString());
        return;
    }
    LDebug("parsing point info finished", __FILE__, __LINE__);
    QByteArray pointInfoByteArray = pointInfoReply->readAll();
    delete pointInfoReply;

    if(liveUrl.toString().indexOf("duowan") == -1) {
        QString rawPointInfoCodeName = getRawPointInfoCodeName(QString::fromUtf8(pointInfoByteArray.mid(0, 500)));
        QString totalPointInfoString = convertAnyCodeToUTF8(pointInfoByteArray, rawPointInfoCodeName);

        addNewPointInfoContentNotFromDuoWan(totalPointInfoString);
    }
    else {
        if(isFirstGetPointInfoFromDuoWan) {
            changeLiveUrlFromDuoWan(pointInfoByteArray);
            startParsingPointInfo();
            return;
        }
        pointInfoByteArray = QString::fromUtf8(pointInfoByteArray).replace("getDataSucc(", "")
                                                                  .replace(QRegExp("\\)$"), "")
                                                                  .toLocal8Bit()
                                                                  .data();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(pointInfoByteArray);
        QJsonObject jsonObj = jsonDoc.object();
        addNewPointInfoContentFromDuoWan(jsonObj);
    }
}

void MatchParser::startParsingMatchInfo() {
    LDebug("start parsing match info", __FILE__, __LINE__);
    if(!matchInfoManager) {
        matchInfoManager = new QNetworkAccessManager();
        connect(matchInfoManager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(parsingMatchInfoFinished(QNetworkReply*)));
    }

    QNetworkRequest request;
    request.setUrl(matchInfoUrl);
    matchInfoManager->get(request);
}

QString MatchParser::getMatchTime() const
{
    return matchTime;
}

QString MatchParser::getMatchStatus() const
{
    return matchStatus;
}

void MatchParser::startParsingPointInfo() {
    LDebug("start parsing point info", __FILE__, __LINE__);
    if(!pointInfoManager) {
        pointInfoManager = new QNetworkAccessManager();
        connect(pointInfoManager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(parsingPointInfoFinished(QNetworkReply*)));
    }

    QNetworkRequest request;
    request.setUrl(liveUrl);
    pointInfoManager->get(request);
}

int MatchParser::getTeamAScore() const {
    return teamAScore;
}

int MatchParser::getTeamBScore() const {
    return teamBScore;
}

QString MatchParser::getTeamAName() const {
    return teamAName;
}

QString MatchParser::getTeamBName() const {
    return teamBName;
}


QUrl MatchParser::getLiveUrl() const {
    return liveUrl;
}

QVector<PointInfoContent *> MatchParser::getPointInfoContents() const {
    return pointInfoContents;
}

QString MatchParser::captureText(QRegExp regx, const QString &totalMatchInfoString) {
    regx.setMinimal(true);
    int pos = regx.indexIn(totalMatchInfoString);
    if(pos > -1) {
        return regx.capturedTexts()[1];
    }
    else {
        return "";
    }
}

QString MatchParser::removeSomeHTMLDom(QString str) {
    QRegExp spanRegx("<span.*>");
    spanRegx.setMinimal(true);
    QRegExp pRegx("<p.*>");
    pRegx.setMinimal(true);
    QRegExp emptyPRegx("<p>[\n\r\t\\s]*</p>");
    emptyPRegx.setMinimal(true);

    return str.replace(QRegExp("[\n\r\t]"), "")
              .replace(spanRegx, "")
              .replace(emptyPRegx, "")
              .replace("&nbsp;", " ")
              .replace("</span>", "")
              .replace("<br>", "")
              .replace("<br/>", "")
              .replace(pRegx, "")
              .replace("</p>", "")
              .trimmed();
}

QStringList MatchParser::findAll(QRegExp regx, const QString &str) {
    regx.setMinimal(true);
    QStringList res;
    int pos = 0;
    while((pos = regx.indexIn(str, pos)) != -1) {
        pos += regx.matchedLength();
        res << regx.cap(1);
    }

    return res;
}

QString MatchParser::getRawPointInfoCodeName(const QString &totalPointInfoString) {
    QRegExp codeNameRegx("charset=\"(\\S+)\">");
    return captureText(codeNameRegx, totalPointInfoString);
}

QString MatchParser::convertAnyCodeToUTF8(QByteArray &bytes, const QString &codeName) {
    if(codeName.toUpper() == "UTF-8") {
        return QString::fromUtf8(bytes);
    }
    else if(codeName.toUpper().indexOf("GB") > -1) {
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        return codec->toUnicode(bytes);
    }
    else {
        return "";
    }
}

void MatchParser::addNewPointInfoContentNotFromDuoWan(const QString &totalPointInfoString) {
    QString totalPointInfoStringWithoutUselessHTMLDom = removeSomeHTMLDom(totalPointInfoString);
    QRegExp pointInfoTimeRegx("<div class=\"live-txt-author\">(.*)</div>");
    QStringList pointInfoTimes = findAll(pointInfoTimeRegx, totalPointInfoStringWithoutUselessHTMLDom);
    QRegExp pointInfoTextRegx("<div class=\"live-txt-info\">(.*)</div>");
    QStringList pointInfoTextsAndImages = findAll(pointInfoTextRegx, totalPointInfoStringWithoutUselessHTMLDom);

    for(int i = 0; i < pointInfoTimes.length() - prePointInfoContentNum; i++) {
        PointInfoContent *pointInfoContent = new PointInfoContent();

        pointInfoContent->time = formatPointInfoTimeNotFromDuoWan(pointInfoTimes[i]);
        pointInfoContent->text = captureTextFromPointInfoTextAndImage(pointInfoTextsAndImages[i]);

        QString pointInfoImageUrl = captureImageUrlFromPointInfoTextAndImage(pointInfoTextsAndImages[i]);
        pointInfoContent->image = pointInfoImageUrl;

        pointInfoContents.append(pointInfoContent);
    }

    prePointInfoContentNum = pointInfoTimes.length();

    emit canGetPointInfo();
}

void MatchParser::addNewPointInfoContentFromDuoWan(const QJsonObject &totalPointInfoJsonObj) {
    QJsonArray pointInfoListArray = totalPointInfoJsonObj.value(QString("list")).toArray();
    for(int i = 0; i < pointInfoListArray.size() - prePointInfoContentNum; i++) {
        PointInfoContent *pointInfoContent = new PointInfoContent();
        QJsonObject pointInfoJsonObj = pointInfoListArray[i].toObject();
        QString msgType = pointInfoJsonObj.value(QString("msg_type")).toString();
        QJsonObject newsObj = pointInfoJsonObj.value(QString("news")).toArray().at(0).toObject();

        if(msgType == "text") {
            QString pointInfoTimeAndText = newsObj.value(QString("txt")).toString();
            pointInfoContent->time = formatPointInfoTimeFromDuoWan(pointInfoTimeAndText);
            pointInfoContent->text = QString(pointInfoTimeAndText).replace(QRegExp("^.*#"), "");
            pointInfoContent->image = "";
        }
        else if(msgType == "image") {
            QString imageUrl = newsObj.value(QString("img")).toObject()
                                                            .value(QString("source")).toString();
            pointInfoContent->image = imageUrl;
            pointInfoContent->time = "";
            pointInfoContent->text = "";
        }

        pointInfoContents.append(pointInfoContent);
    }
    for(int i = 0; i < pointInfoListArray.size() - 1; i++) {
        PointInfoContent *p = pointInfoContents[i];
        if(p->text == "" && p->time == "") {
            p->time = pointInfoContents[i + 1]->time;
            p->text = "比赛截图";
        }
    }
    prePointInfoContentNum = pointInfoListArray.size();
    emit canGetPointInfo();
}

void MatchParser::changeLiveUrlFromDuoWan(const QByteArray &pointInfoByteArray) {
    isFirstGetPointInfoFromDuoWan = false;
    int actID = getActIDFromDuoWan(QString::fromUtf8(pointInfoByteArray));
    liveUrl = QUrl(QString("https://lol.duowan.com/wxapi2017s7/index.php?"
                           "r=api/ajaxmsg&callback=getDataSucc&"
                           "actid=%1&pageSize=500&time=0").arg(actID));
}

QString MatchParser::formatPointInfoTimeNotFromDuoWan(const QString &pointInfoTime) {
    QRegExp timeRegx("(\\d+).*(\\d+)\\s*(\\S+)");
    timeRegx.indexIn(pointInfoTime);
    QStringList times = timeRegx.capturedTexts();
    return QString("%1 : %2  %3").arg(times[1])
                                 .arg(times[2])
                                 .arg(times[3]);
}

QString MatchParser::formatPointInfoTimeFromDuoWan(const QString &pointInfoTime) {
    QRegExp timeRegx("\\s*#\\s*(\\d+)\\s*#\\s*(\\d+)[：:](\\d+)\\s*#\\s*");
    timeRegx.indexIn(pointInfoTime);
    QStringList times = timeRegx.capturedTexts();
    QString ch = "一二三四五";
    if(times[1].toInt() == 0) {
        return "";
    }
    return QString("%1 : %2  第%3场").arg(times[2])
                                     .arg(times[3])
                                     .arg(ch[times[1].toInt() - 1]);
}

QString MatchParser::captureTextFromPointInfoTextAndImage(const QString &pointInfoTextAndImage) {
    QRegExp imageRegx("<img.*>");
    imageRegx.setMinimal(true);
    return QString(pointInfoTextAndImage).replace(imageRegx, "");
}

QString MatchParser::captureImageUrlFromPointInfoTextAndImage(const QString &pointInfoTextAndImage) {
    QRegExp imageUrlRegx("(src|data-original)=\"(.*)\"");
    imageUrlRegx.setMinimal(true);
    imageUrlRegx.indexIn(pointInfoTextAndImage);
    return imageUrlRegx.capturedTexts()[2].replace(QRegExp("^//"), "https://");
}

int MatchParser::getActIDFromDuoWan(const QString &totalPointInfoString) {
    QRegExp actIDRegx("actid: (\\d+),");
    actIDRegx.setMinimal(true);
    actIDRegx.indexIn(totalPointInfoString);
    return actIDRegx.capturedTexts()[1].toInt();
}
