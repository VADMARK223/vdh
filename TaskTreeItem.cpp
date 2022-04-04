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

[[maybe_unused]] QString TaskTreeItem::toString() const {
    QString addressString;
    QTextStream qTextStream(&addressString);
    qTextStream << this;

    return "TaskTreeItem(" +
           this->data(ID_INDEX).toString() + "," +
           this->data(DEPTH_INDEX).toString() + "," +
            addressString +
           ")";
}

int TaskTreeItem::getId() const {
    return this->data(ID_INDEX).toInt();
}

int TaskTreeItem::getParentId() const {
    return this->data(PARENT_ID_INDEX).toInt();
}

QVector<QVariant> TaskTreeItem::getItemData() {
    return _itemData;
}
/*
QModelIndex TaskTreeItem::index(int row, int column, const QModelIndex &parent) const {
    return QModelIndex();
}

QModelIndex TaskTreeItem::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int TaskTreeItem::rowCount(const QModelIndex &parent) const {
    return 0;
}

int TaskTreeItem::columnCount(const QModelIndex &parent) const {
    return 0;
}

QVariant TaskTreeItem::data(const QModelIndex &index, int role) const {
    return QVariant();
}*/
