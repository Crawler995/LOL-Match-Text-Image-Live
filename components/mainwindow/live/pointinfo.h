#ifndef POINTINFO_H
#define POINTINFO_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPixmap>
#include <QVBoxLayout>

class PointInfo : public QWidget {
    Q_OBJECT
public:
    explicit PointInfo(QWidget *parent = 0);

    void setTime(const QString &time);
    void setText(const QString &text);
    void setImageUrl(const QString &imageUrl);

    bool isInVisibleArea(int value);
    void loadImage();

    QString getText() const;

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *pointInfoAreaParent;
    QVBoxLayout *layout;

    QString imageUrl;
    QLabel *timeLabel;
    QLabel *textLabel;
    QLabel *imageLabel;

    bool alreadyBecomeTrueSize;

    void initStyles();


public slots:
    void dynamicLoadImage(int sliderValue);

signals:
    void becomeTrueSize();
};

#endif // POINTINFO_H
