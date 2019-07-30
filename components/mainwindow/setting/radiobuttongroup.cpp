#include "radiobuttongroup.h"
#include "features/colorboard.h"

#include <QRadioButton>

RadioButtonGroup::RadioButtonGroup(const QStringList &selections, QWidget *parent)
    : QWidget(parent)
{
    group = new QButtonGroup(this);
    layout = new QVBoxLayout(this);

    for(auto selection = selections.constBegin(); selection != selections.constEnd(); selection++) {
        QRadioButton *button = new QRadioButton(*selection, this);
        button->setStyleSheet("QRadioButton{"
                              "font-size: 14px;"
                              "color: " + ColorBoard::gold5 + ";}"
                              "QRadioButton:checked{"
                              "color: " + ColorBoard::gold3 + ";}"
                              "QRadioButton::indicator{"
                              "width: 12px; height: 12px; margin-right: 4px;"
                              "border: 2px solid " + ColorBoard::gold5 + ";"
                              "border-radius: 7px;}"
                              "QRadioButton::indicator:unchecked{"
                              "background:" + ColorBoard::gold5 + ";}"
                              "QRadioButton::indicator:checked{"
                              "width: 12px; height: 12px;"
                              "border: 2px solid " + ColorBoard::gold5 + ";"
                              "background:" + ColorBoard::gold3 + ";}");
        group->addButton(button);
        group->setId(button, selection - selections.constBegin());
        layout->addWidget(button);
    }
    layout->addSpacing(20);
}

void RadioButtonGroup::setDefaultSelection(int id) {
    group->button(id)->setChecked(true);
}

void RadioButtonGroup::setAllButtonEnabled(bool enabled) {
    for(auto i = group->buttons().begin(); i != group->buttons().end(); i++) {
        (*i)->setEnabled(enabled);
    }
}

QButtonGroup *RadioButtonGroup::getGroup() const {
    return group;
}
