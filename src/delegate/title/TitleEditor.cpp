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

QString TitleEditor::getText() {
    return _lineEdit->text();
}

void TitleEditor::setText(const QString& value) {
    _lineEdit->setText(value);
    _lineEdit->selectAll();
}

bool TitleEditor::getChecked() {
    return _checkBox->isChecked();
}

void TitleEditor::setChecked(bool value) {
    _checkBox->setChecked(value);
}