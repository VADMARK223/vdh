//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_TITLEDELEGATE_H
#define VDH_TITLEDELEGATE_H


#include <QStyledItemDelegate>

class TitleDelegate : public QStyledItemDelegate {
Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *
    createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};


#endif //VDH_TITLEDELEGATE_H
