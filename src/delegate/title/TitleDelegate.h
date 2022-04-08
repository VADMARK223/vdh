//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_TITLEDELEGATE_H
#define VDH_TITLEDELEGATE_H


#include <QStyledItemDelegate>
#include <QtWidgets>

const int CHECK_ROLE = Qt::UserRole + 1;

class TitleDelegate : public QStyledItemDelegate {
Q_OBJECT

public:
    explicit TitleDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *
    createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void
    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

//    QCheckBox *_checkBox;
//    QLabel *mRight;
//    QFrame *mFrame;
};


#endif //VDH_TITLEDELEGATE_H
