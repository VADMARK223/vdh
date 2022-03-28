//
// Created by v.markitanov on 27.03.2022.
//

#include <iostream>
#include "objectTreeModel.h"

using namespace std;

ObjectTreeModel::ObjectTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    _rootItem = new QObject(this);
}

void ObjectTreeModel::setColumns(QStringList cols) {
    _columns = cols;
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    QObject *parentObject = objByIndex(parent);
    return createIndex(row, column, parentObject->children().at(row));
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &child) const {
    QObject *childObj = objByIndex(child);
    QObject *parentObj = childObj->parent();
    if (parentObj == _rootItem) {
        return QModelIndex();
    }

    QObject *grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);
    return createIndex(row, 0, parentObj);
}

int ObjectTreeModel::rowCount(const QModelIndex &parent) const {
    return objByIndex(parent)->children().count();
}

int ObjectTreeModel::columnCount(const QModelIndex &parent) const {
//    Q_UNUSED(parent);
    return _columns.count();
}

QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const QByteArray &name = _columns.at(index.column()).toUtf8();
        return objByIndex(index)->property(name);
    }

    return QVariant();

}

QObject *ObjectTreeModel::objByIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return _rootItem;
    return static_cast<QObject*>(index.internalPointer());
}

void ObjectTreeModel::addItem(QObject *item, const QModelIndex &parentIndex) {
    beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));
    item->setParent(objByIndex(parentIndex));
    endInsertRows();
}

