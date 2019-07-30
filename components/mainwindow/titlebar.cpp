#include "titlebar.h"
#include "features/colorboard.h"
#include "features/setting/settinghandler.h"

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QIcon>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>

TitleBar::TitleBar(const QString &titleText,
                   QWidget *mainWidget) :
    QWidget(mainWidget)
{
    isMouseLeftBtnPressDown = false;

    titleLabel = new QLabel(titleText, this);
    minBtn = new QPushButton(this);
    closeBtn = new QPushButton(this);

    setFixedHeight(40);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    minBtn->setFixedSize(26, 26);
    closeBtn->setFixedSize(26, 26);

    connect(minBtn, SIGNAL(clicked(bool)), this, SLOT(minuimize()));
    connect(closeBtn, SIGNAL(clicked(bool)), this, SLOT(close()));

    layout = new QHBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(minBtn);
    layout->addWidget(closeBtn);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    setStyleSheet("QPushButton{background: transparent; border: 0px;}"
                  "QLabel{color: " + ColorBoard::gold4 + ";}");
    minBtn->setStyleSheet("QPushButton{border-image: url(:/icons/images/icons/minus.png);}"
                          "QPushButton:hover{border-image: url(:/icons/images/icons/minus-hover.png);}"
                          "QPushButton:pressed{border-image: url(:/icons/images/icons/minus-press.png);}");
    closeBtn->setStyleSheet("QPushButton{border-image: url(:/icons/images/icons/close.png);}"
                          "QPushButton:hover{border-image: url(:/icons/images/icons/close-hover.png);}"
                          "QPushButton:pressed{border-image: url(:/icons/images/icons/close-press.png);}");

}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        isMouseLeftBtnPressDown = true;
        startDragPoint = event->globalPos();
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        isMouseLeftBtnPressDown = false;
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if(!isMouseLeftBtnPressDown || window()->isMaximized()) {
        return;
    }

    QWidget *mainWindow = parentWidget()->parentWidget();
    mainWindow->move(mainWindow->geometry().topLeft() + event->globalPos() - startDragPoint);
    startDragPoint = event->globalPos();
}

void TitleBar::minuimize() {
    window()->showMinimized();
}

void TitleBar::close() {
    window()->close();
}
