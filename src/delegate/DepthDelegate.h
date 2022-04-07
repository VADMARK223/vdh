//
// Created by v.markitanov on 06.04.2022.
//

#ifndef VDH_DEPTHDELEGATE_H
#define VDH_DEPTHDELEGATE_H


#include <QStyledItemDelegate>

class DepthDelegate : public QStyledItemDelegate {
Q_OBJECT
public:
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        return QStyledItemDelegate::sizeHint(option, index);
    }

    QWidget *
    createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void
    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    void commitAndCloseEditor();
};


#endif //VDH_DEPTHDELEGATE_H
