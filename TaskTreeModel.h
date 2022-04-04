//
// Created by v.markitanov on 28.03.2022.
//

#ifndef VDH_TASKTREEMODEL_H
#define VDH_TASKTREEMODEL_H

#include <QStringList>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QFile>
#include <QTreeView>

#include "TaskTreeItem.h"

class TaskTreeModel : public QAbstractItemModel {
public:
    int lastUniqueId = 0;

    explicit TaskTreeModel(QObject *parent = nullptr);

    ~TaskTreeModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void setModelData(QFile *file);

    TaskTreeItem *getRootItem();

    TaskTreeItem *insertTask(int row, bool isSubTask, const QModelIndex &parent = QModelIndex());

    bool insertRow(int row, const QModelIndex &parent = QModelIndex());

    QModelIndex indexFromItem(TaskTreeItem *item);

protected:

    TaskTreeItem *rootItem;
};

#endif //VDH_TASKTREEMODEL_H
