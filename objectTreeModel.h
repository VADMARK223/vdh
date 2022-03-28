//
// Created by v.markitanov on 27.03.2022.
//

#ifndef VDH_OBJECTTREEMODEL_H
#define VDH_OBJECTTREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QStandardItemModel>

class ObjectTreeModel : public QAbstractItemModel {
Q_OBJECT
public:
    explicit ObjectTreeModel(QObject *parent = nullptr);

    void setColumns(QStringList cols);
    void addItem(QObject *item, const QModelIndex &parentIndex);

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;

protected:
    QObject *_rootItem;
    QStringList _columns;
    [[nodiscard]] QObject *objByIndex(const QModelIndex &index) const;
};

#endif //VDH_OBJECTTREEMODEL_H
