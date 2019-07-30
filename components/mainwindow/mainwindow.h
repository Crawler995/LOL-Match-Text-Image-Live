#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "titlebar.h"

#include <QWidget>
#include <QPoint>
#include <QString>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSystemTrayIcon>
#include <QEvent>
#include <QGraphicsDropShadowEffect>

enum Direction {RIGHT, RIGHT_BOTTOM, BOTTOM, OTHERS};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(const QString appName,
                        QWidget *parent = 0);

protected:
    const int DRAG_WIDTH;
    const int SHADOW_WIDTH;

    QGraphicsDropShadowEffect *shadow;
    QGridLayout *wrapLayout;
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    TitleBar *titleBar;
    QWidget *contentWidget;

    bool isMouseLeftBtnPressDown;
    QPoint dragPoint;
    Direction dragDir;
    QSystemTrayIcon *systemTrayIcon;

    void setWindowSize();
    void setWindowStyle();

    void initWindowComponent(QWidget *contentWidget);

    void dragDirectionHandle(const QPoint &cursorGlobalPoint);

    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);

    void minToSystemTrayIcon();

protected slots:
    void systemTrayIconToNormalWindow(QSystemTrayIcon::ActivationReason reason);
};

#endif // MAINWINDOW_H
