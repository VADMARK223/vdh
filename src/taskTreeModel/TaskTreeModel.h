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

    ~TaskTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    void setupModelData(const QStringList &lines, TaskTreeItem *parent);

    TaskTreeItem *rootItem;
};

#endif //VDH_TASKTREEMODEL_H
