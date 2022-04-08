//
// Created by v.markitanov on 07.04.2022.
//

#include "TitleDelegate.h"
#include "../../data/TitleData.h"
#include "TitleEditor.h"
#include <QLineEdit>
#include <QApplication>
#include <QPainter>
#include <QEvent>

TitleDelegate::TitleDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void TitleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();

    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    if (index.data().canConvert<TitleData>()) {
        auto titleData = qvariant_cast<TitleData>(index.data());
        style->drawItemText(painter,
                            QRect(opt.rect.x() + 20, opt.rect.y(), opt.rect.width() - 20, opt.rect.height()),
                            Qt::AlignVCenter,
                            QApplication::palette(),
                            true,
                            titleData.getTitle(),
                            option.state & QStyle::State_Selected ? QPalette::HighlightedText
                                                                  : QPalette::Text);

        QStyleOptionButton cbOpt;
        cbOpt.rect = QRect(option.rect.x(), option.rect.y(), option.rect.width(), option.rect.height());

        if (titleData.getDone()) {
            cbOpt.state |= QStyle::State_On;
        } else {
            cbOpt.state |= QStyle::State_Off;
        }

        QApplication::style()->drawControl(QStyle::CE_CheckBox, &cbOpt, painter);
    }
}

QWidget *
TitleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *editor = new TitleEditor(parent);
    return editor;
}

void TitleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *titleEditor = dynamic_cast<TitleEditor *>(editor);

    if (index.data().canConvert<TitleData>()) {
        auto titleData = qvariant_cast<TitleData>(index.data());
        titleEditor->setText(titleData.getTitle());
        titleEditor->setChecked(titleData.getDone());
    }
}

void TitleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto *titleEditor = dynamic_cast<TitleEditor *>(editor);

    if (index.data().canConvert<TitleData>()) {
        auto titleData = qvariant_cast<TitleData>(index.data());
        titleData.setTitle(titleEditor->getText());
        titleData.setDone(titleEditor->getChecked());
        model->setData(index, QVariant::fromValue(titleData));
    }
}

void TitleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
    editor->setGeometry(
            QRect(option.rect.x() - 10, option.rect.y() - 10, 300,
                  editor->geometry().height() + 10));
}

QSize TitleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QSize s = index.data(Qt::SizeHintRole).toSize();
    return s.isValid() ? s : QStyledItemDelegate::sizeHint(option, index);
}

bool TitleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                const QModelIndex &index) {
    if (event->type() != QEvent::MouseButtonRelease && event->type() != QEvent::MouseButtonDblClick) {
        return false;
    }

    const auto *const mouseEvent = dynamic_cast<const QMouseEvent *>(event);
    int mousePosX = mouseEvent->pos().x();
    int optionRectX = option.rect.x();
    if (index.data().canConvert<TitleData>() && (event->type() == QEvent::MouseButtonRelease) &&
        mousePosX >= optionRectX && mousePosX <= (optionRectX + 10)) {
        QStyleOptionButton cbOpt;
        cbOpt.rect = option.rect;

        auto titleData = qvariant_cast<TitleData>(index.data());
        titleData.setDone(!titleData.getDone());
        model->setData(index, QVariant::fromValue(titleData));

        return true;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
//    QStyleOptionProgressBar styleOptionProgressBar;
//    styleOptionProgressBar.minimum = 0;
//    styleOptionProgressBar.maximum = 10;
//    styleOptionProgressBar.rect = option.rect;
//    styleOptionProgressBar.progress = 2;
//    styleOptionProgressBar.text = QString::number(3);
//    styleOptionProgressBar.textVisible = true;
//    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &styleOptionProgressBar, painter);
}
