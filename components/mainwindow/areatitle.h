#ifndef AREATITLE_H
#define AREATITLE_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>

class AreaTitle : public QWidget {
    Q_OBJECT
public:
    explicit AreaTitle(const QString &text, QWidget *parent = 0);

private:
    QLabel *leftLineLabel, *textLabel, *rightLineLabel;
    QHBoxLayout *layout;
};

#endif // AREATITLE_H
