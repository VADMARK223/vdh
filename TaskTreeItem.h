//
// Created by v.markitanov on 28.03.2022.
//

#ifndef VDH_TASKTREEITEM_H
#define VDH_TASKTREEITEM_H

#include <QVariant>
#include <QAbstractItemModel>

class TaskTreeItem {
public:
    explicit TaskTreeItem(const QVector<QVariant> &data, TaskTreeItem *parentItem = nullptr);

    ~TaskTreeItem();

    void appendChild(TaskTreeItem *item);

    void appendChildAtRow(int row, TaskTreeItem *item);

    void removeChildren();

    void removeChildAtRow(int row);

    TaskTreeItem *child(int row);

    [[nodiscard]] int childCount() const;

    [[nodiscard]] int columnCount() const;

    [[nodiscard]] QVariant data(int column) const;

    TaskTreeItem *parentItem();

    [[nodiscard]] int row() const;

    [[maybe_unused]] [[nodiscard]] QString toString() const;

    [[nodiscard]] int getId() const;

    [[nodiscard]] int getParentId() const;

    [[nodiscard]] QString getTitle() const;

    [[nodiscard]] QString getComments() const;

    QVector<QVariant> getItemData();

    void clear();

private:
    QVector<TaskTreeItem *> _childItems;
    QVector<QVariant> _itemData;
    TaskTreeItem *_parentItem;
};

#endif //VDH_TASKTREEITEM_H
