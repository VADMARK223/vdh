//
// Created by v.markitanov on 07.04.2022.
//

#include "TitleDelegate.h"
#include <QLineEdit>
#include <QApplication>

void TitleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *
TitleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(option.rect);
    return lineEdit;
}

void TitleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *lineEdit = dynamic_cast<QLineEdit*>(editor);
    lineEdit->setText(index.data().toString());
}

void TitleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QStyledItemDelegate::setModelData(editor, model, index);
}
