#include "mainwindow.h"
#include "titlebar.h"
#include "features/colorboard.h"
#include "../../features/setting/settinghandler.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QString>
#include <QMouseEvent>
#include <QRect>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

MainWindow::MainWindow(const QString appName,
                       QWidget *parent) :
    QWidget(parent),
    DRAG_WIDTH(3),
    SHADOW_WIDTH(0)
{
    mainWidget = new QWidget(this);
    titleBar = new TitleBar(appName, mainWidget);

    setMouseTracking(true);
    mainWidget->setMouseTracking(true);
    isMouseLeftBtnPressDown = false;
    dragDir = OTHERS;

    setWindowSize();
    setWindowStyle();
}

void MainWindow::setWindowSize() {
    int screenWidth = QApplication::desktop()->screenGeometry().width();
    int screenHeight = QApplication::desktop()->screenGeometry().height();

    resize(screenWidth * 2 / 3, screenHeight * 2 / 3);
}

void MainWindow::setWindowStyle() {
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    mainWidget->setObjectName("mainWidget");
//    mainWidget->setStyleSheet("background-color:" + ColorBoard::black1 + ";");
//    mainWidget->setStyleSheet("background: url(:/images/images/bg-1.jpg);");
//    shadow = new QGraphicsDropShadowEffect(mainWidget);
//    shadow->setBlurRadius(SHADOW_WIDTH);
//    shadow->setOffset(0, 0);
//    mainWidget->setGraphicsEffect(shadow);
}

void MainWindow::initWindowComponent(QWidget *contentWidget) {
    mainLayout = new QVBoxLayout(mainWidget);

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(contentWidget);
    mainLayout->setSpacing(0);

    int margin = 5 * DRAG_WIDTH;
    mainLayout->setContentsMargins(margin, 0, margin, margin);
    mainWidget->setLayout(mainLayout);

    wrapLayout = new QGridLayout(this);
    wrapLayout->addWidget(mainWidget);
    wrapLayout->setContentsMargins(SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH);
    setLayout(wrapLayout);

    setStyleSheet("QWidget#mainWidget{border-image: url(:/images/images/bg-1.jpg);}");
}

void MainWindow::dragDirectionHandle(const QPoint &cursorGlobalPoint) {
    QRect rect = this->rect();
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    bool isDragRight = x >= rb.x() - DRAG_WIDTH - SHADOW_WIDTH && x <= rb.x();
    bool isDragBottom = y >= rb.y() - DRAG_WIDTH - SHADOW_WIDTH && y <= rb.y();

    if(isDragRight && isDragBottom) {
        dragDir = RIGHT_BOTTOM;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(isDragRight) {
        dragDir = RIGHT;
        setCursor(Qt::SizeHorCursor);
    }
    else if(isDragBottom) {
        dragDir = BOTTOM;
        setCursor(Qt::SizeVerCursor);
    }
    else {
        dragDir = OTHERS;
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        isMouseLeftBtnPressDown = false;

        if(dragDir != OTHERS) {
            releaseMouse();
            setCursor(Qt::ArrowCursor);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        isMouseLeftBtnPressDown = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!isMouseLeftBtnPressDown) {
        dragDirectionHandle(gloPoint);
    }
    else {
        if(dragDir == OTHERS) {
            return;
        }

        QRect target(tl, rb);
        switch(dragDir) {
        case RIGHT:
            target.setWidth(gloPoint.x() - tl.x() + SHADOW_WIDTH);
            break;
        case BOTTOM:
            target.setHeight(gloPoint.y() - tl.y() + SHADOW_WIDTH);
            break;
        case RIGHT_BOTTOM:
            target.setWidth(gloPoint.x() - tl.x() + SHADOW_WIDTH);
            target.setHeight(gloPoint.y() - tl.y() + SHADOW_WIDTH);
            break;
        default:
            break;
        }

        setGeometry(target);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    SettingHandler settingHandler;
    if(settingHandler.getCloseSetting() == RUN_BACK) {
        minToSystemTrayIcon();
        event->ignore();
    }
}

void MainWindow::hideEvent(QHideEvent *event) {
    event->ignore();
}

void MainWindow::minToSystemTrayIcon() {
    window()->hide();
    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/icons/images/icons/minus.png"));
    systemTrayIcon->setToolTip(tr("LOL Match Live\n单击显示主界面"));
    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayIconToNormalWindow(QSystemTrayIcon::ActivationReason)));
    systemTrayIcon->show();
}

void MainWindow::systemTrayIconToNormalWindow(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger) {
        systemTrayIcon->hide();
        delete systemTrayIcon;
        window()->show();
    }
}
