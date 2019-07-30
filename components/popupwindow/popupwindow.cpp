#include "popupwindow.h"
#include "features/colorboard.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPropertyAnimation>

PopupWindow::PopupWindow(int maxInfoNum, QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    this->maxInfoNum = maxInfoNum;

    tooMany = false;
    screenWidth = QApplication::desktop()->screenGeometry().width();
    screenHeight = QApplication::desktop()->screenGeometry().height();

    layout = new QVBoxLayout(this);
    layout->setSpacing(0);

    titleLabel = new QLabel("LOL Match Live", this);
    layout->addWidget(titleLabel);

    setLayout(layout);
    initStyles();
}

void PopupWindow::addPopupPointInfo(PopupPointInfo *popuppointInfo) {
    layout->insertWidget(1, popuppointInfo);
    setLayout(layout);
}

void PopupWindow::initStyles() {
    setStyleSheet("background-color: " + ColorBoard::black1 + ";"
                  "margin: 0px;"
                  "padding-left: 6px;"
                  "padding-right: 6px;");
    titleLabel->setStyleSheet("color: " + ColorBoard::gold3 + ";"
                              "font-size: 16px");

    setFixedWidth(screenWidth / 6);
    move(screenWidth, 0);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void PopupWindow::popup() {
    if(tooMany) {
        tooManyLabel = new QLabel("……新赛点过多，\n可打开主界面查看", this);
        tooManyLabel->setAlignment(Qt::AlignCenter);
        tooManyLabel->setStyleSheet("color: " + ColorBoard::gold3 + ";"
                                    "font-size: 14px;");
        layout->addWidget(tooManyLabel);
        setLayout(layout);
    }

    this->show();

    startAni = new QPropertyAnimation(this, "pos");
    startAni->setDuration(1000);
    startAni->setEndValue(QPoint(screenWidth * 5 / 6, 0));
    startAni->setEasingCurve(QEasingCurve::InOutCubic);
    startAni->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(5000, this, SLOT(fadeOut()));
}

void PopupWindow::setTooMany(bool value)
{
    tooMany = value;
}

int PopupWindow::getMaxInfoNum() const {
    return maxInfoNum;
}

void PopupWindow::fadeOut() {
    endAni = new QPropertyAnimation(this, "pos");
    endAni->setDuration(1000);
    endAni->setEndValue(QPoint(screenWidth, 0));
    endAni->setEasingCurve(QEasingCurve::InOutCubic);
    endAni->start(QAbstractAnimation::DeleteWhenStopped);

    connect(endAni, SIGNAL(finished()), this, SLOT(close()));
}
