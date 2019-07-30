#include "areatitle.h"
#include "features/colorboard.h"

#include <QString>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

AreaTitle::AreaTitle(const QString &text, QWidget *parent) : QWidget(parent) {
    leftLineLabel = new QLabel(this);
    textLabel = new QLabel(text, this);
    rightLineLabel = new QLabel(this);

    leftLineLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightLineLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    leftLineLabel->setMaximumSize(60, 2);
    leftLineLabel->setMinimumSize(10, 2);
    rightLineLabel->setMaximumSize(60, 2);
    rightLineLabel->setMinimumSize(10, 2);

    leftLineLabel->setStyleSheet("background-color: " + ColorBoard::gold2 + ";");
    leftLineLabel->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
                                 "stop: 0 " + ColorBoard::gold2 + ","
                                 "stop: 1 " + ColorBoard::convertHexColorToRGBA(ColorBoard::gold2, 0) + ");");
    rightLineLabel->setStyleSheet("background-color: qlineargradient(x1: 1, y1: 0, x2: 0, y2: 0,"
                                 "stop: 0 " + ColorBoard::gold2 + ","
                                 "stop: 1 " + ColorBoard::convertHexColorToRGBA(ColorBoard::gold2, 0) + ");");

    textLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = textLabel->font();
    titleFont.setBold(true);
    titleFont.setLetterSpacing(QFont::PercentageSpacing, 140);
    textLabel->setFont(titleFont);
    textLabel->setStyleSheet("color: " + ColorBoard::gold5 + ";");

    layout = new QHBoxLayout(this);
    layout->addWidget(leftLineLabel);
    layout->addWidget(textLabel);
    layout->addWidget(rightLineLabel);

    setLayout(layout);
}
