//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"

TaskTreeModel::TaskTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    qDebug("Task tree model constructor.");
    _rootItem = new QObject(this);
}

void TaskTreeModel::setColumns(QStringList cols) {
    _columns = cols;
}

QModelIndex TaskTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return {};
    }
    QObject *parentObject = objByIndex(parent);
    return createIndex(row, column, parentObject->children().at(row));
}

QModelIndex TaskTreeModel::parent(const QModelIndex &child) const {
    QObject *childObj = objByIndex(child);
    QObject *parentObj = childObj->parent();
    if (parentObj == _rootItem) {
        return {};
    }

    QObject *grandParentObj = parentObj->parent();
    int row = static_cast<int>(grandParentObj->children().indexOf(parentObj));
    return createIndex(row, 0, parentObj);
}

int TaskTreeModel::rowCount(const QModelIndex &parent) const {
    return static_cast<int>(objByIndex(parent)->children().count());
}

int TaskTreeModel::columnCount(const QModelIndex &parent) const {
    return static_cast<int>(_columns.count());
}

QVariant TaskTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    if (role == Qt::DisplayRole) {
        const QByteArray &name = _columns.at(index.column()).toUtf8();
        return objByIndex(index)->property(name);
    }

    return {};
}

QObject *TaskTreeModel::objByIndex(const QModelIndex &index) const {
    if (!index.isValid())
        return _rootItem;
    return static_cast<QObject *>(index.internalPointer());
}

void TaskTreeModel::addItem(QObject *item, const QModelIndex &parentIndex) {
    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));
    item->setParent(objByIndex(parentIndex));
    endInsertRows();
}
