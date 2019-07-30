#ifndef POINTINFOAREA_H
#define POINTINFOAREA_H

#include "pointinfo.h"

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>
#include <QResizeEvent>
#include <QEvent>
#include <QObject>
#include <QMovie>

class PointInfoArea : public QScrollArea {
    Q_OBJECT
public:
    explicit PointInfoArea(QWidget *parent = 0);

    void addPointInfo(PointInfo *pointInfo);
    void clearAllPointInfo();
    void clearNetworkErrorInfo();

    int getPrePointInfoNum() const;
    void setPrePointInfoNum(int value);

    void connectScrollBarMoveSignal();
    void initVisibleImage();

    int getAreaHeight() const;

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *contentWidget;
    QVBoxLayout *layout;
    int prePointInfoNum;
    QVector<PointInfo*> pointInfos;

    int areaHeight;

    void initStyles();

signals:
    void initFinished();
};

#endif // POINTINFOAREA_H
