//
// Created by v.markitanov on 28.03.2022.
//

#ifndef VDH_TASKTREEITEM_H
#define VDH_TASKTREEITEM_H

#include <QVariant>

class TaskTreeItem {
public:
    explicit TaskTreeItem(const QVector<QVariant> &data, TaskTreeItem *parentItem = nullptr);

    ~TaskTreeItem();

    void appendChild(TaskTreeItem *item);

    TaskTreeItem *child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    TaskTreeItem *parentItem();

    int row() const;

private:
    QVector<TaskTreeItem *> _childItems;
    QVector<QVariant> _itemData;
    TaskTreeItem *_parentItem;
};

#endif //VDH_TASKTREEITEM_H
