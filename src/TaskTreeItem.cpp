//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeItem.h"
#include "ColumnsData.h"
#include "data/TitleData.h"

TaskTreeItem::TaskTreeItem(const QVector<QVariant> &data, TaskTreeItem *parentItem)
        : _itemData(data), _parentItem(parentItem) {
}

TaskTreeItem::~TaskTreeItem() {
    qDeleteAll(_childItems);
}

void TaskTreeItem::appendChild(TaskTreeItem *item) {
    _childItems.append(item);
}

void TaskTreeItem::appendChildAtRow(int row, TaskTreeItem *item) {
    _childItems.insert(row, item);
}

void TaskTreeItem::removeChildren() {
    _childItems.clear();
}

void TaskTreeItem::removeChildAtRow(int row) {
    _childItems.removeAt(row);
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
           this->data(ColumnsData::getIndexByAlias(TITLE_ALIAS)).toString() + "," +
           this->data(ColumnsData::getIndexByAlias(ID_ALIAS)).toString() + "," +
           this->data(ColumnsData::getIndexByAlias(DEPTH_ALIAS)).toString() + "," +
           addressString +
           ")";
}

int TaskTreeItem::getId() const {
    return this->data(ColumnsData::getIndexByAlias(ID_ALIAS)).toInt();
}

int TaskTreeItem::getParentId() const {
    return this->data(ColumnsData::getIndexByAlias(PARENT_ID_ALIAS)).toInt();
}

int TaskTreeItem::getPriority() const {
    return this->data(ColumnsData::getIndexByAlias(PRIORITY_ALIAS)).toInt();
}

int TaskTreeItem::getStar() const {
    return this->data(ColumnsData::getIndexByAlias(STAR_ALIAS)).toInt();
}

QString TaskTreeItem::getTitle() const {
    auto titleData = qvariant_cast<TitleData>(this->data(ColumnsData::getIndexByAlias(TITLE_ALIAS)));
    return titleData.getTitle();
}

bool TaskTreeItem::getDone() const {
    auto titleData = qvariant_cast<TitleData>(this->data(ColumnsData::getIndexByAlias(TITLE_ALIAS)));
    return titleData.getDone();
}

QString TaskTreeItem::getComments() const {
    return this->data(ColumnsData::getIndexByAlias(COMMENTS_ALIAS)).toString();
}

QVector<QVariant> TaskTreeItem::getItemData() {
    return _itemData;
}

void TaskTreeItem::clear() {
    _childItems.clear();
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
