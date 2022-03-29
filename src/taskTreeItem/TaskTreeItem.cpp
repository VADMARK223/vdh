//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeItem.h"

TaskTreeItem::TaskTreeItem(const QVector<QVariant> &data, TaskTreeItem *parentItem)
        : _itemData(data), _parentItem(parentItem) {
}

TaskTreeItem::~TaskTreeItem() {
    qDeleteAll(_childItems);
}

void TaskTreeItem::appendChild(TaskTreeItem *item) {
    _childItems.append(item);
}

TaskTreeItem *TaskTreeItem::child(int row) {
    if (row < 0 || row >= _childItems.size()) {
        return nullptr;
    }

    return _childItems.at(row);
}

int TaskTreeItem::childCount() const {
    return static_cast<int>(_childItems.count());
}

int TaskTreeItem::columnCount() const {
    return static_cast<int>(_itemData.count());
}

QVariant TaskTreeItem::data(int column) const {
    if (column < 0 || column >= _itemData.size()) {
        return {};
    }

    return _itemData.at(column);
}

TaskTreeItem *TaskTreeItem::parentItem() {
    return _parentItem;
}

int TaskTreeItem::row() const {
    if (_parentItem) {
        return static_cast<int>(_parentItem->_childItems.indexOf(const_cast<TaskTreeItem *>(this)));
    }
    return 0;
}

