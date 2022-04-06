//
// Created by v.markitanov on 06.04.2022.
//

#include "DepthDelegate.h"
#include <QApplication>

QWidget *
DepthDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *editor = new QSlider(parent);
    editor->setMinimum(0);
    editor->setMaximum(10);
    editor->setOrientation(Qt::Horizontal);
    editor->setAutoFillBackground(true);
    return editor;
}

void DepthDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *slider = dynamic_cast<QSlider *>(editor);
    slider->setValue(index.data().toInt());
}

void DepthDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto *slider = dynamic_cast<QSlider *>(editor);
    model->setData(index, slider->value());
}

void DepthDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}

void DepthDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    int value = index.data().toInt();
    QStyleOptionProgressBar styleOptionProgressBar;
    styleOptionProgressBar.minimum = 0;
    styleOptionProgressBar.maximum = 10;
    styleOptionProgressBar.rect = option.rect;
    styleOptionProgressBar.progress = value;
    styleOptionProgressBar.text = QString::number(value);
    styleOptionProgressBar.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &styleOptionProgressBar, painter);
}
