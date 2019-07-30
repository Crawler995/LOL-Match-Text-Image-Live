#ifndef POPUPPOINTINFO_H
#define POPUPPOINTINFO_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>

class PopupPointInfo : public QWidget {
    Q_OBJECT
public:
    explicit PopupPointInfo(QWidget *parent = 0);

    void setTime(const QString &time);
    void setText(const QString &text);

private:
    QVBoxLayout *layout;

    QLabel *timeLabel;
    QLabel *textLabel;

    void initStyles();
};

#endif // POPUPPOINTINFO_H
