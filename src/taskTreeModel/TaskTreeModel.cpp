//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"

#include <QStringList>

TaskTreeModel::TaskTreeModel(const QString &data, QObject *parent)
        : QAbstractItemModel(parent) {
    rootItem = new TaskTreeItem({tr("Title"), tr("Summary")});
    setupModelData(data.split('\n'), rootItem);
}

TaskTreeModel::~TaskTreeModel() {
    delete rootItem;
}

int TaskTreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<TaskTreeItem *>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant TaskTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TaskTreeItem *item = static_cast<TaskTreeItem *>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TaskTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TaskTreeModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TaskTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TaskTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TaskTreeItem *>(parent.internalPointer());

    TaskTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TaskTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    TaskTreeItem *childItem = static_cast<TaskTreeItem *>(index.internalPointer());
    TaskTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TaskTreeModel::rowCount(const QModelIndex &parent) const {
    TaskTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TaskTreeItem *>(parent.internalPointer());

    return parentItem->childCount();
}

void TaskTreeModel::setupModelData(const QStringList &lines, TaskTreeItem *parent) {
    QVector<TaskTreeItem *> parents;
    QVector<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        qDebug() << "position: " << position;

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings =
                    lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);

            qDebug() << "columnStrings: " << columnStrings;

            QVector<QVariant> columnData;
            columnData.reserve(columnStrings.count());


            for (const QString &columnString: columnStrings)
                columnData << columnString;
            qDebug() << "columnData: " << columnData;

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount() - 1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TaskTreeItem(columnData, parents.last()));
        }
        ++number;
    }

    QVector<QVariant> columnData;
    columnData << QList<QVariant>({QVariant("a"), QVariant("b")});
    parents.last()->appendChild(new TaskTreeItem(columnData, parents.last()));

    QVector<QVariant> columnData1;
    columnData1 << QList<QVariant>({QVariant("c"), QVariant("d")});
    rootItem->appendChild(new TaskTreeItem(columnData1, rootItem));
}
