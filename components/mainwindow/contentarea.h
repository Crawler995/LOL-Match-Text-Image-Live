#ifndef CONTENTAREA_H
#define CONTENTAREA_H

#include "live/livearea.h"
#include "setting/settingarea.h"

#include <QWidget>
#include <QGridLayout>

class ContentArea : public QWidget {
    Q_OBJECT
public:
    explicit ContentArea(QWidget *parent = 0);

private:
    LiveArea *liveArea;
    SettingArea *settingArea;

    QGridLayout *layout;
};

#endif // CONTENTAREA_H
