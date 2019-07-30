#ifndef SETTINGAREA_H
#define SETTINGAREA_H

#include "../areatitle.h"
#include "settingtitle.h"
#include "radiobuttongroup.h"
#include "../../../features/setting/settinghandler.h"
#include "matchidinput.h"

#include <QWidget>
#include <QVBoxLayout>

class SettingArea : public QWidget {
    Q_OBJECT
public:
    explicit SettingArea(QWidget *parent = 0);

    MatchIDInput *getMatchIDInput() const;

private:
    QVBoxLayout *layout;
    AreaTitle *title;

    SettingTitle *inputMatchIDTitle;
    MatchIDInput *matchIDInput;

    SettingTitle *closeSettingTitle;
    RadioButtonGroup *closeSettingButtonGroup;
    SettingTitle *whenHasNewPointInfoSettingTitle;
    RadioButtonGroup *whenHasNewPointInfoSettingButtonGroup;

    SettingHandler *settingHandler;

    void setDefaultSelection();
    void connectButtonToSettings();

private slots:
    void startParsingMatchInfoSlot();

signals:
    void startParsingMatchInfoSignal(int matchID);
};

#endif // SETTINGAREA_H
