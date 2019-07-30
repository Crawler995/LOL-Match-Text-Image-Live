#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "popuppointinfo.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>

class PopupWindow : public QWidget {
    Q_OBJECT
public:
    explicit PopupWindow(int maxInfoNum = 2, QWidget *parent = 0);

    void addPopupPointInfo(PopupPointInfo *popuppointInfo);
    void popup();

    void setTooMany(bool value);

    int getMaxInfoNum() const;

private:
    QVBoxLayout *layout;
    QLabel *titleLabel, *tooManyLabel;
    int screenWidth, screenHeight;
    QPropertyAnimation *startAni, *endAni;
    bool tooMany;
    int maxInfoNum;

    void initStyles();

private slots:
    void fadeOut();
};

#endif // POPUPWINDOW_H
