#ifndef SETTINGHANDLER_H
#define SETTINGHANDLER_H

#include <QSettings>
#include <QString>
#include <QObject>

enum CLOSE_SETTING { EXIT, RUN_BACK };
enum WHEN_HAS_NEW_POINT_INFO_SETTING { POPUP_SHOW, NO_SHOW };

class SettingHandler : public QObject {
public:
    SettingHandler(const QString &organization = QString("Crawler"),
                   const QString &application = QString("LOL Match Live"),
                   QObject *parent = Q_NULLPTR);

public slots:
    CLOSE_SETTING getCloseSetting();
    WHEN_HAS_NEW_POINT_INFO_SETTING getWhenHasNewPointInfoSetting();

    void setCloseSetting(CLOSE_SETTING setting);
    void setWhenHasNewPointInfoSetting(WHEN_HAS_NEW_POINT_INFO_SETTING setting);

private:
    QSettings *settings;
};

#endif // SETTINGHANDLER_H
