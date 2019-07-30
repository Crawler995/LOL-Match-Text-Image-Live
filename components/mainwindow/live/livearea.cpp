#include "livearea.h"
#include "pointinfo.h"
#include "features/colorboard.h"
#include "features/fontloader.h"
#include "features/setting/settinghandler.h"
#include "../../popupwindow/popupwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QThread>
#include <QVector>
#include <QPixmap>
#include <QTime>
#include <QNetworkReply>
#include <QDebug>

LiveArea::LiveArea(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);

    title = new AreaTitle("MATCH LIVE", this);
    pointInfoArea = new PointInfoArea(this);
    matchInfo = new MatchInfo(this);

    connect(pointInfoArea, SIGNAL(initFinished()),
            matchInfo, SLOT(clearParsingGif()));

    layout->addWidget(title);
    layout->addWidget(matchInfo);
    layout->addWidget(pointInfoArea);

    setLayout(layout);

    parseThread = nullptr;
}

LiveArea::~LiveArea() {
    deletePreParseThread();
}

MatchParser *LiveArea::getMatchInfoParser() const {
    return matchParser;
}

void LiveArea::startParsingMatchInfo(int matchID) {
    if(parseThread) {
        deletePreParseThread();
        delete matchParser;
    }

    pointInfoArea->clearAllPointInfo();
    matchInfo->loadParsingGif();

    matchParser = new MatchParser(matchID);
    parseThread = new QThread(this);

    matchParser->moveToThread(parseThread);
    connect(matchParser, SIGNAL(canGetMatchInfo()),
            this, SLOT(setMatchInfo()), Qt::QueuedConnection);
    connect(matchParser, SIGNAL(canGetPointInfo()),
            this, SLOT(setPointInfo()), Qt::QueuedConnection);
    connect(matchParser, SIGNAL(getInfoNetworkError(QString)),
            this, SLOT(showNetworkError(QString)), Qt::QueuedConnection);
    parseThread->start();

    matchParser->startParsingMatchAndPointInfo();
}

void LiveArea::deletePreParseThread() {
    if(!parseThread) {
        return;
    }
    parseThread->exit();
    parseThread->wait();
    delete parseThread;
}

void LiveArea::setMatchInfo() {
    matchInfo->setTeamAName(matchParser->getTeamAName());
    matchInfo->setTeamBName(matchParser->getTeamBName());
    matchInfo->setTeamAScore(matchParser->getTeamAScore());
    matchInfo->setTeamBScore(matchParser->getTeamBScore());
    matchInfo->setMatchTime(matchParser->getMatchTime());
    matchInfo->setMatchStatus(matchParser->getMatchStatus());
}

void LiveArea::setPointInfo() {
    pointInfoArea->clearNetworkErrorInfo();

    QVector<PointInfoContent*> pointInfoContents = matchParser->getPointInfoContents();
    int prePointInfoNum = pointInfoArea->getPrePointInfoNum();

    SettingHandler settingHandler;
    bool needPopupWindow = settingHandler.getWhenHasNewPointInfoSetting() == POPUP_SHOW &&
                           pointInfoContents.length() - prePointInfoNum > 0;

    PopupWindow *popupWindow;
    int popupNum;
    int popupMaxInfoNum;
    if(needPopupWindow) {
        popupWindow = new PopupWindow();
        popupNum = 0;
        popupMaxInfoNum = popupWindow->getMaxInfoNum();
    }

    for(int i = pointInfoContents.length() - 1; i >= prePointInfoNum; i--) {
        PointInfo *pointInfo = new PointInfo(pointInfoArea);
        pointInfo->setTime(pointInfoContents[i]->time);
        pointInfo->setText(pointInfoContents[i]->text);
        pointInfo->setImageUrl(pointInfoContents[i]->image);
        pointInfoArea->addPointInfo(pointInfo);

        if(needPopupWindow && popupNum++ >= popupMaxInfoNum) {
            popupWindow->setTooMany(true);
        }
        if(needPopupWindow && i < prePointInfoNum + popupMaxInfoNum) {
            PopupPointInfo *popupPointInfo = new PopupPointInfo(popupWindow);
            popupPointInfo->setTime(pointInfoContents[i]->time);
            popupPointInfo->setText(pointInfoContents[i]->text + (pointInfoContents[i]->image == "" ?
                                                                 "" : "\n（图）"));
            popupWindow->addPopupPointInfo(popupPointInfo);
        }
    }

    if(needPopupWindow) {
        popupWindow->popup();
    }

    pointInfoArea->setPrePointInfoNum(pointInfoContents.length());
    pointInfoArea->connectScrollBarMoveSignal();
    pointInfoArea->initVisibleImage();
}

void LiveArea::showNetworkError(const QString &error) {
    PointInfo *errorInfo = new PointInfo(pointInfoArea);
    QString errorTime = QString("%1时%2分").arg(QTime::currentTime().hour())
                                           .arg(QTime::currentTime().minute());
    QString errorText = "访问错误，错误原因：\n" + error;
    errorInfo->setTime(errorTime);
    errorInfo->setText(errorText);
    pointInfoArea->addPointInfo(errorInfo);

    SettingHandler settingHandler;
    bool needPopupWindow = settingHandler.getWhenHasNewPointInfoSetting() == POPUP_SHOW;

    PopupWindow *popupWindow;
    if(needPopupWindow) {
        popupWindow = new PopupWindow();
        PopupPointInfo *popupErrorInfo = new PopupPointInfo(popupWindow);
        popupErrorInfo->setTime(errorTime);
        popupErrorInfo->setText(errorText);
        popupWindow->addPopupPointInfo(popupErrorInfo);

        popupWindow->popup();
    }
}
