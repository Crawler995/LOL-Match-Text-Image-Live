#include "settinghandler.h"

#include <QVariant>

SettingHandler::SettingHandler(const QString &organization,
                               const QString &application,
                               QObject *parent)
{
    settings = new QSettings(organization, application, parent);

    if(settings->value("close") == QVariant()) {
        setCloseSetting(RUN_BACK);
    }
    if(settings->value("new_point_info") == QVariant()) {
        setWhenHasNewPointInfoSetting(POPUP_SHOW);
    }
}

CLOSE_SETTING SettingHandler::getCloseSetting() {
    return CLOSE_SETTING(settings->value("close").toInt());
}

WHEN_HAS_NEW_POINT_INFO_SETTING SettingHandler::getWhenHasNewPointInfoSetting() {
    return WHEN_HAS_NEW_POINT_INFO_SETTING(settings->value("new_point_info").toInt());
}


void SettingHandler::setCloseSetting(CLOSE_SETTING setting) {
    settings->setValue("close", QVariant(setting));
}

void SettingHandler::setWhenHasNewPointInfoSetting(WHEN_HAS_NEW_POINT_INFO_SETTING setting) {
    settings->setValue("new_point_info", QVariant(setting));
}





