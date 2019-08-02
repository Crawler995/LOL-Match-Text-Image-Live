#include "pointinfo.h"
#include "pointinfoarea.h"
#include "features/colorboard.h"

#include <QFont>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QEventLoop>
#include <QRect>
#include <QScrollArea>
#include <QScrollBar>
#include <QMovie>
#include <QDebug>

PointInfo::PointInfo(QWidget *parent) : QWidget(parent) {
    this->pointInfoAreaParent = parent;

    layout = new QVBoxLayout(this);
    layout->setSpacing(0);

    timeLabel = new QLabel(this);
    textLabel = new QLabel(this);
    textLabel->setTextFormat(Qt::PlainText);
    imageLabel = nullptr;

    layout->addWidget(timeLabel);
    layout->addWidget(textLabel);

    setLayout(layout);
    initStyles();

    alreadyBecomeTrueSize = false;
}

void PointInfo::setTime(const QString &time) {
    timeLabel->setText(time);
}

void PointInfo::setText(const QString &text) {
    textLabel->setText(text);
}

void PointInfo::setImageUrl(const QString &imageUrl) {
    this->imageUrl = imageUrl;
}

void PointInfo::dynamicLoadImage(int sliderValue) {
    if(imageLabel || imageUrl == "") {
        return;
    }

    if(!alreadyBecomeTrueSize) {
        QEventLoop loop;
        connect(this, SIGNAL(becomeTrueSize()), &loop, SLOT(quit()));
        loop.exec();
    }

    if(!isInVisibleArea(sliderValue)) {
        return;
    }

    imageLabel = new QLabel(this);
    layout->addWidget(imageLabel);
    setLayout(layout);

    loadImage();

    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setStyleSheet("padding-bottom: 10px;");
}

void PointInfo::initStyles() {
    QFont timeFont = timeLabel->font();
    timeFont.setPixelSize(14);
    timeFont.setBold(true);
    timeLabel->setFont(timeFont);

    QFont textFont = textLabel->font();
    textFont.setPixelSize(14);
    textLabel->setFont(textFont);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignTop);

    setStyleSheet("background-color: " + ColorBoard::convertHexColorToRGBA(ColorBoard::black2, 0.4) + ";"
                  "margin: 0px;"
                  "padding-left: 10px;"
                  "padding-right: 10px;");
    timeLabel->setStyleSheet("color: " + ColorBoard::gold3 + ";"
                             "border-width: 0px;"
                             "padding-top: 10px;");
    textLabel->setStyleSheet("color: " + ColorBoard::gold5 + ";"
                             "border-width: 0px;"
                             "padding-bottom: 10px;"
                             "padding-top: 10px;");
}

void PointInfo::loadImage() {
    QMovie *loading = new QMovie(":/icons/images/icons/spinner-1.gif");
    imageLabel->setMovie(loading);
    loading->start();

    QUrl url(imageUrl);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray jpegData = reply->readAll();
    delete reply;
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);

    loading->stop();
    delete loading;

    if(pixmap.width() > timeLabel->width()) {
        double scale = 0.9 * timeLabel->width() / pixmap.width();
        pixmap = pixmap.scaled(pixmap.size() * scale, Qt::KeepAspectRatio);
    }
    imageLabel->setPixmap(pixmap);
}

QString PointInfo::getText() const {
    return textLabel->text();
}

void PointInfo::resizeEvent(QResizeEvent *event) {
    if(!alreadyBecomeTrueSize) {
        emit becomeTrueSize();
        alreadyBecomeTrueSize = true;
    }
}

bool PointInfo::isInVisibleArea(int value) {
    QRect rect = this->geometry();
    int areaHeight = qobject_cast<PointInfoArea*>(this->pointInfoAreaParent)->getAreaHeight();

    return rect.top() > value &&
           rect.bottom() < value + areaHeight;
}
