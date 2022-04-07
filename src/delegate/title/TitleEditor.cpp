//
// Created by v.markitanov on 07.04.2022.
//

#include "TitleEditor.h"

TitleEditor::TitleEditor(QWidget *parent) : QWidget(parent), _checkBox(new QCheckBox), _lineEdit(new QLineEdit("Test")) {
    auto *layout = new QHBoxLayout();
    layout->addWidget(_checkBox);
    layout->addWidget(_lineEdit);
    setLayout(layout);
}