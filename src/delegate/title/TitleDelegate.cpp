//
// Created by v.markitanov on 07.04.2022.
//

#include "TitleDelegate.h"
#include "../../data/TitleData.h"
#include <QLineEdit>
#include <QApplication>
#include <QPainter>

void TitleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    if (index.data().canConvert<TitleData>()) {
        auto titleData = qvariant_cast<TitleData>(index.data());
        style->drawItemText(painter,
                            option.rect,
                            Qt::AlignVCenter,
                            QApplication::palette(),
                            true,
                            titleData.getTitle(),
                            option.state & QStyle::State_Selected ? QPalette::HighlightedText
                                                                  : QPalette::Text);
    } else {
        style->drawItemText(painter, option.rect, Qt::AlignVCenter, QApplication::palette(), true,
                            index.data().toString(),
                            option.state & QStyle::State_Selected ? QPalette::HighlightedText
                                                                  : QPalette::Text);
    }
}

QWidget *
TitleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *lineEdit = new QLineEdit(parent);
    lineEdit->setGeometry(option.rect);
    return lineEdit;
}

void TitleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *lineEdit = dynamic_cast<QLineEdit *>(editor);
    if (index.data().canConvert<TitleData>()) {
        auto titleData = qvariant_cast<TitleData>(index.data());
        lineEdit->setText(titleData.getTitle());
    }
}

void TitleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto *lineEdit = dynamic_cast<QLineEdit *>(editor);

    if (index.data().canConvert<TitleData>()) {
        auto titleData = qvariant_cast<TitleData>(index.data());
        titleData.setTitle(lineEdit->text());
        model->setData(index, QVariant::fromValue(titleData));
    }
}
