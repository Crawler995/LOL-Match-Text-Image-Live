#ifndef MATCHIDINPUT_H
#define MATCHIDINPUT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVector>
#include <QLabel>

class MatchIDInput : public QWidget {
    Q_OBJECT
public:
    explicit MatchIDInput(QWidget *parent = 0);

    QPushButton *getSubmitMatchIDButton() const;
    QLineEdit *getMatchIDLineEdit() const;
    void getTodayMatch();

private:
    QGridLayout *layout;
    QLineEdit *matchIDLineEdit;
    QPushButton *submitMatchIDButton;
    QLabel *todayMatchLabel;
    QVector<QPushButton*> todayMatchButton;
    QVector<int> matchIDs;

    void autoInputMatchID(int index);
};

#endif // MATCHIDINPUT_H
