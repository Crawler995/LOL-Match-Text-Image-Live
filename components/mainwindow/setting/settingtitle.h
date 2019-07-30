#ifndef SETTINGTITLE_H
#define SETTINGTITLE_H

#include <QWidget>
#include <QLabel>
#include <QString>

class SettingTitle : public QLabel {
    Q_OBJECT
public:
    explicit SettingTitle(const QString &title, QWidget *parent = 0);
};

#endif // SETTINGTITLE_H
