//
// Created by v.markitanov on 28.03.2022.
//

#ifndef VDH_TASKTREEMODEL_H
#define VDH_TASKTREEMODEL_H

#include <QStandardItemModel>
#include "../taskTreeItem/TaskTreeItem.h"

class TaskTreeModel : public QAbstractItemModel {
public:
    explicit TaskTreeModel(const QString &data, QObject *parent = nullptr);

    ~TaskTreeModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

protected:
    void setupModelData(const QStringList &lines, TaskTreeItem *parent);

    TaskTreeItem *rootItem;
};

#endif //VDH_TASKTREEMODEL_H
