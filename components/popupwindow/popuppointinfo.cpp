#include "popuppointinfo.h"
#include "features/colorboard.h"
#include <QFont>
#include <QUrl>
#include <QPixmap>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMovie>

PopupPointInfo::PopupPointInfo(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);

    timeLabel = new QLabel(this);
    textLabel = new QLabel(this);

    layout->addWidget(timeLabel);
    layout->addWidget(textLabel);

    setLayout(layout);
    initStyles();
}

void PopupPointInfo::initStyles() {
    QFont timeFont = timeLabel->font();
    timeFont.setPixelSize(14);
    timeFont.setBold(true);
    timeLabel->setFont(timeFont);

    QFont textFont = textLabel->font();
    textFont.setPixelSize(14);
    textLabel->setFont(textFont);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignTop);

    setStyleSheet("background-color: " + ColorBoard::black2 + ";"
                  "margin: 0px;");
    timeLabel->setStyleSheet("color: " + ColorBoard::gold3 + ";"
                             "border-width: 0px;"
                             "padding-top: 4px;");
    textLabel->setStyleSheet("color: " + ColorBoard::gold5 + ";"
                             "border-width: 0px;"
                             "padding-bottom: 4px;"
                             "padding-top: 4px;");
}

void PopupPointInfo::setTime(const QString &time) {
    timeLabel->setText(time);
}

void PopupPointInfo::setText(const QString &text) {
    textLabel->setText(text);
}


