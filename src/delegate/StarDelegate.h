//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_STARDELEGATE_H
#define VDH_STARDELEGATE_H

#include <QStyledItemDelegate>

class StarDelegate : public QStyledItemDelegate {
Q_OBJECT
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif //VDH_STARDELEGATE_H
