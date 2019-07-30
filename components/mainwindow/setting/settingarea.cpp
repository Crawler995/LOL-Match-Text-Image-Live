#include "settingarea.h"
#include "features/colorboard.h"

#include <QStringList>
#include <QRegExp>
#include <QValidator>

SettingArea::SettingArea(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);
    title = new AreaTitle("SETTINGS", this);

    inputMatchIDTitle = new SettingTitle("输入比赛ID：");
    matchIDInput = new MatchIDInput(this);
    connect(matchIDInput->getMatchIDLineEdit(), SIGNAL(returnPressed()),
            this, SLOT(startParsingMatchInfoSlot()));

    closeSettingTitle = new SettingTitle("点击右上角关闭按钮时：", this);
    QStringList closeSettingList;
    closeSettingList << "彻底关闭软件"
                     << "驻在后台";
    closeSettingButtonGroup = new RadioButtonGroup(closeSettingList, this);

    whenHasNewPointInfoSettingTitle = new SettingTitle("当窗口最小化或驻在后台，比赛有新动态时：", this);
    QStringList whenHasNewPointInfoSettingList;
    whenHasNewPointInfoSettingList << "弹窗提示"
                            << "不提示";
    whenHasNewPointInfoSettingButtonGroup = new RadioButtonGroup(whenHasNewPointInfoSettingList, this);

    layout->addWidget(title);
    layout->addWidget(inputMatchIDTitle);
    layout->addWidget(matchIDInput);
    layout->addSpacing(26);

    layout->addWidget(closeSettingTitle);
    layout->addWidget(closeSettingButtonGroup);
    layout->addWidget(whenHasNewPointInfoSettingTitle);
    layout->addWidget(whenHasNewPointInfoSettingButtonGroup);
    layout->addStretch();

    setLayout(layout);

    connectButtonToSettings();
    setDefaultSelection();
}

MatchIDInput *SettingArea::getMatchIDInput() const {
    return matchIDInput;
}

void SettingArea::setDefaultSelection() {
    closeSettingButtonGroup->setDefaultSelection(settingHandler->getCloseSetting());
    whenHasNewPointInfoSettingButtonGroup->setDefaultSelection(settingHandler->getWhenHasNewPointInfoSetting());
}

void SettingArea::connectButtonToSettings() {
    settingHandler = new SettingHandler();

    connect(closeSettingButtonGroup->getGroup(),
            static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            settingHandler, [=](int id){ settingHandler->setCloseSetting(CLOSE_SETTING(id)); });
    connect(whenHasNewPointInfoSettingButtonGroup->getGroup(),
            static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            settingHandler,
            [=](int id){ settingHandler->setWhenHasNewPointInfoSetting(WHEN_HAS_NEW_POINT_INFO_SETTING(id)); });

    connect(matchIDInput->getSubmitMatchIDButton(), SIGNAL(clicked(bool)),
            this, SLOT(startParsingMatchInfoSlot()));
}

void SettingArea::startParsingMatchInfoSlot() {
    int matchID = matchIDInput->getMatchIDLineEdit()->text().toInt();
    emit startParsingMatchInfoSignal(matchID);
}
