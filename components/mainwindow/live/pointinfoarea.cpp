#include "pointinfoarea.h"
#include "pointinfo.h"
#include "features/colorboard.h"
#include "features/utils.h"

#include <QWidget>
#include <QScrollArea>
#include <QDebug>
#include <QScrollBar>
#include <QLayoutItem>
#include <QPixmap>

PointInfoArea::PointInfoArea(QWidget *parent) : QScrollArea(parent) {
    contentWidget = new QWidget(this);
    setWidget(contentWidget);

    layout = new QVBoxLayout(this->widget());
    layout->setSpacing(0);

    layout->addStretch();
    this->widget()->setLayout(layout);
    setWidgetResizable(true);

    initStyles();

    prePointInfoNum = 0;
}

void PointInfoArea::addPointInfo(PointInfo *pointInfo) {
    layout->insertWidget(0, pointInfo, 0, Qt::AlignTop);
    this->widget()->setLayout(layout);
    this->widget()->adjustSize();
    pointInfos.push_front(pointInfo);
}

void PointInfoArea::clearAllPointInfo() {
    QLayoutItem *child;
    while((child = layout->takeAt(0)) != 0) {
        if(child->widget()) {
            child->widget()->setParent(nullptr);
        }
        delete child;
    }
    prePointInfoNum = 0;

    pointInfos.clear();
}

void PointInfoArea::clearNetworkErrorInfo() {
    QLayoutItem *errorInfo;
    while((errorInfo = layout->itemAt(0)) != 0) {
        if(!qobject_cast<PointInfo*>(errorInfo->widget())) {
            break;
        }
        if(qobject_cast<PointInfo*>(errorInfo->widget())->getText().indexOf("网络连接错误") == -1) {
            break;
        }
        layout->takeAt(0);
        errorInfo->widget()->setParent(nullptr);
        delete errorInfo;
    }
}

int PointInfoArea::getPrePointInfoNum() const {
    return prePointInfoNum;
}

void PointInfoArea::setPrePointInfoNum(int value) {
    LDebug(QString("point info num: %1 -> %2").arg(prePointInfoNum).arg(value), __FILE__, __LINE__);
    prePointInfoNum = value;
}

void PointInfoArea::connectScrollBarMoveSignal() {
    for(auto i = pointInfos.begin(); i != pointInfos.end(); i++) {
        connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)),
                *i, SLOT(dynamicLoadImage(int)));
    }
}

void PointInfoArea::initVisibleImage() {
    for(auto i = pointInfos.begin(); i != pointInfos.end(); i++) {
        (*i)->dynamicLoadImage(0);
    }
    emit initFinished();
}

int PointInfoArea::getAreaHeight() const {
    return areaHeight;
}

void PointInfoArea::resizeEvent(QResizeEvent *event) {
    areaHeight = this->viewport()->geometry().height();
    this->widget()->resize(this->viewport()->geometry().width(), this->widget()->height());
}

void PointInfoArea::initStyles() {
    QString halfTransBlack = ColorBoard::convertHexColorToRGBA(ColorBoard::black1, 0.4);
    setStyleSheet("QScrollArea{margin-top: 0px;"
                  "border: 1px solid " + ColorBoard::gold1 + ";"
                  "background-color: " + halfTransBlack + ";}");
//    this->widget()->setStyleSheet("background-color: " + halfTransBlack + ";");
    this->widget()->setStyleSheet("background: transparent;");
    this->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical{background:transparent; width: 14px;}"
        "QScrollBar::handle:vertical{background:" + ColorBoard::gold1 + ";"
        "border:2px solid transparent;"
        "border-radius:4px;}"
        "QScrollBar::handle:hover{background:" + ColorBoard::gold2 + ";}"
        "QScrollBar::sub-line:vertical{background:transparent;}"
        "QScrollBar::add-line:vertical{background:transparent;}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "background: none;}"
    );
    this->horizontalScrollBar()->setStyleSheet(
        "QScrollBar:horizontal{background:transparent; height: 14px;}"
        "QScrollBar::handle:horizontal{background:" + ColorBoard::gold1 + ";"
        "border:2px solid transparent;"
        "border-radius:4px;}"
        "QScrollBar::handle:hover{background:" + ColorBoard::gold2 + ";}"
        "QScrollBar::sub-line:horizontal{background:transparent;}"
        "QScrollBar::add-line:horizontal{background:transparent;}"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
        "background: none;}"
    );
}
