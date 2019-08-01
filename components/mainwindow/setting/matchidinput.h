#ifndef MATCHIDINPUT_H
#define MATCHIDINPUT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVector>
#include <QNetworkReply>
#include <QLabel>

class MatchIDInput : public QWidget {
    Q_OBJECT
public:
    explicit MatchIDInput(QWidget *parent = 0);

    QPushButton *getSubmitMatchIDButton() const;
    QLineEdit *getMatchIDLineEdit() const;

private:
    QGridLayout *layout;
    QLineEdit *matchIDLineEdit;
    QPushButton *submitMatchIDButton;
    QLabel *todayMatchLabel;
    QVector<QPushButton*> todayMatchButton;
    QVector<int> matchIDs;

    void startGetTodayMatch();
    void autoInputMatchID(int index);

private slots:
    void getTodayMatchFinished(QNetworkReply* reply);
};

#endif // MATCHIDINPUT_H
