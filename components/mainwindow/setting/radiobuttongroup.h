#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QStringList>
#include <QVBoxLayout>

class RadioButtonGroup : public QWidget {
    Q_OBJECT
public:
    explicit RadioButtonGroup(const QStringList &selections, QWidget *parent = 0);

    void setDefaultSelection(int id);
    void setAllButtonEnabled(bool enabled);

    QButtonGroup *getGroup() const;

private:
    QButtonGroup *group;
    QVBoxLayout *layout;
};

#endif // RADIOBUTTONGROUP_H
