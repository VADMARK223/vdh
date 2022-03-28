//
// Created by v.markitanov on 27.03.2022.
//

#ifndef VDH_OBJECTTREEMODEL_H
#define VDH_OBJECTTREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class ObjectTreeModel : public QAbstractItemModel {
Q_OBJECT
public:
    explicit ObjectTreeModel(QObject *parent = nullptr);

//    void setColumns(QStringList *cols);
    void setColumns(QStringList cols);
    void addItem(QObject *item, const QModelIndex &parentIndex);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QObject *_rootItem;
    QStringList _columns;
    QObject *objByIndex(const QModelIndex &index) const;
};

#endif //VDH_OBJECTTREEMODEL_H
