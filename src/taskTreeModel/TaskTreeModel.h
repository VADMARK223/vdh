//
// Created by v.markitanov on 28.03.2022.
//

#ifndef VDH_TASKTREEMODEL_H
#define VDH_TASKTREEMODEL_H

#include <QStandardItemModel>

class TaskTreeModel : public QAbstractItemModel {
public:
    explicit TaskTreeModel(QObject *parent = nullptr);

    void setColumns(QStringList cols);
    void addItem(QObject *item, const QModelIndex &parentIndex);

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

protected:
    QObject *_rootItem;
    QStringList _columns;
    [[nodiscard]] QObject *objByIndex(const QModelIndex &index) const;
};

#endif //VDH_TASKTREEMODEL_H
