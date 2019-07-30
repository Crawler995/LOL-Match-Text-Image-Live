#include "matchidinput.h"
#include "../../../features/colorboard.h"

#include <QRegExp>
#include <QValidator>
#include <QRegExpValidator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>

MatchIDInput::MatchIDInput(QWidget *parent) : QWidget(parent) {
    layout = new QGridLayout(this);

    matchIDLineEdit = new QLineEdit(this);
    submitMatchIDButton = new QPushButton("确定", this);

    layout->addWidget(matchIDLineEdit, 0, 0, 1, 4);
    layout->addWidget(submitMatchIDButton, 0, 4, 1, 2);

    todayMatchLabel = new QLabel("正在查询今日比赛……", this);
    todayMatchLabel->setStyleSheet("font-size: 14px; color: " + ColorBoard::gold5 + ";");
    layout->addWidget(todayMatchLabel, 1, 0, 1, 6);

    matchIDLineEdit->setStyleSheet("QLineEdit{background-color:" + ColorBoard::gold5 + ";"
                                   "border: 1px solid " + ColorBoard::gold3 + ";}");
    matchIDLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QRegExp regx("[0-9]+$");
    QValidator *numValidator = new QRegExpValidator(regx, matchIDLineEdit);
    matchIDLineEdit->setValidator(numValidator);

    setStyleSheet("QPushButton{background-color:" + ColorBoard::gold3 + ";"
                  "font-size: 14px;"
                  "border: none;"
                  "padding: 4px 10px 4px 10px;"
                  "color:" + ColorBoard::gold5 + ";}"
                  "QPushButton:hover{background-color:" + ColorBoard::gold4 + ";"
                  "color: " + ColorBoard::gold1 + ";}"
                  "QPushButton:pressed{background-color:" + ColorBoard::gold2 + ";"
                  "color: " + ColorBoard::gold1 + ";}");
    submitMatchIDButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    submitMatchIDButton->setFlat(true);

    setLayout(layout);

    getTodayMatch();
}

QPushButton *MatchIDInput::getSubmitMatchIDButton() const {
    return submitMatchIDButton;
}

QLineEdit *MatchIDInput::getMatchIDLineEdit() const {
    return matchIDLineEdit;
}

void MatchIDInput::getTodayMatch() {
    QUrl url("http://qt.qq.com/php_cgi/lol_mobile/gamecenter/varcache_index.php?plat=android&version=9891");
    QEventLoop loop;
    QNetworkAccessManager manager;
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    loop.exec();

    if(reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        todayMatchLabel->setText("获取今日比赛失败 ×");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    delete reply;
    QJsonObject obj = doc.object();

    QJsonArray arr = obj.value(QString("live_list")).toArray();
    for(auto i = arr.constBegin(); i != arr.constEnd(); i++) {
        QJsonObject liveObj = (*i).toObject();
        int matchID = liveObj.value(QString("bMatchId")).toString().toInt();
        QString teamName = liveObj.value(QString("bMatchName")).toString();
        QString matchTime = liveObj.value(QString("MatchDate")).toString();
        int index = i - arr.constBegin();

        QPushButton *button = new QPushButton(this);
        todayMatchButton.append(button);
        matchIDs.append(matchID);

        connect(button, &QPushButton::clicked,
                this, [=]() { autoInputMatchID(index); });

        button->setText(teamName + "\n" + matchTime);

        layout->addWidget(button, index / 2 + 2, index % 2 * 3, 1, 3);
    }

    setLayout(layout);

    todayMatchLabel->setText("今日比赛如下，点击可自动填写比赛ID：");
}

void MatchIDInput::autoInputMatchID(int index) {
    matchIDLineEdit->setText(QString::number(matchIDs.at(index)));
}