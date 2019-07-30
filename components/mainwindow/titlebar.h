#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QPoint>
#include <QMouseEvent>

class TitleBar : public QWidget {
    Q_OBJECT
public:
    explicit TitleBar(const QString &titleText,
                      QWidget *mainWidget);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QLabel *titleLabel;
    QPushButton *minBtn, *closeBtn;
    QHBoxLayout *layout;

    bool isMouseLeftBtnPressDown;
    QPoint startDragPoint;

protected slots:
    void minuimize();
    void close();
};

#endif // TITLEBAR_H
