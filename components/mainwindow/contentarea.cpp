#include "contentarea.h"
#include "live/livearea.h"
#include "setting/settingarea.h"

#include <QWidget>
#include <QGridLayout>

ContentArea::ContentArea(QWidget *parent) : QWidget(parent) {
    liveArea = new LiveArea(this);
    settingArea = new SettingArea(this);

    layout = new QGridLayout(this);
    layout->addWidget(liveArea, 0, 0, 1, 2);
    layout->addWidget(settingArea, 0, 2, 1, 1);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    connect(settingArea, SIGNAL(startParsingMatchInfoSignal(int)),
            liveArea, SLOT(startParsingMatchInfo(int)));
}
