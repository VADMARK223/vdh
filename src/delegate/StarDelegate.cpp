//
// Created by v.markitanov on 07.04.2022.
//

#include "StarDelegate.h"

void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
}
