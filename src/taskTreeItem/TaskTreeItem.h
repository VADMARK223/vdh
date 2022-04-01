//
// Created by v.markitanov on 28.03.2022.
//

#ifndef VDH_TASKTREEITEM_H
#define VDH_TASKTREEITEM_H

#include <QVariant>

#define ID_INDEX 0
#define PARENT_ID_INDEX 1
#define DEPTH_INDEX 2
#define COMMENTS_INDEX 3

class TaskTreeItem {
public:
    explicit TaskTreeItem(const QVector<QVariant> &data, TaskTreeItem *parentItem = nullptr);

    ~TaskTreeItem();

    void appendChild(TaskTreeItem *item);

    TaskTreeItem *child(int row);

    [[nodiscard]] int childCount() const;

    [[nodiscard]] int columnCount() const;

    [[nodiscard]] QVariant data(int column) const;

    TaskTreeItem *parentItem();

    [[nodiscard]] int row() const;

    [[maybe_unused]] [[nodiscard]] QString toString() const;

    [[nodiscard]] int getId() const;

    [[nodiscard]] int getParentId() const;

    QVector<QVariant> getItemData();

    QVector<TaskTreeItem *> _childItems;
private:
    QVector<QVariant> _itemData;
    TaskTreeItem *_parentItem;
};

#endif //VDH_TASKTREEITEM_H
