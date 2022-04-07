//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"
#include "ColumnsData.h"


TaskTreeModel::TaskTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    QVector<QVariant> data = {};

    for (const auto &column: ColumnsData::getColumns()) {
        data << QVariant(column.name);
    }

    rootItem = new TaskTreeItem(data);
}

TaskTreeModel::~TaskTreeModel() {
    delete rootItem;
}

int TaskTreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return static_cast<TaskTreeItem *>(parent.internalPointer())->columnCount();
    }

    return rootItem->columnCount();
}

QVariant TaskTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    switch (role) {
        case Qt::DisplayRole: {
            auto *item = static_cast<TaskTreeItem *>(index.internalPointer());
            return item->data(index.column());
        }

        case Qt::SizeHintRole: {
            return QSize(0, 20);
        }

            /*case Qt::BackgroundRole: {
                auto *item = static_cast<TaskTreeItem *>(index.internalPointer());
                if (item->getId() > 3) {
                    return QBrush(Qt::green);
                } else {
                    return QBrush(Qt::gray);
                }
            }*/

        default:
            return {};
    }
}

Qt::ItemFlags TaskTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    if (index.column() == ColumnsData::getIndexByAlias(TITLE_ALIAS) ||
        index.column() == ColumnsData::getIndexByAlias(DEPTH_ALIAS) ||
            index.column() == ColumnsData::getIndexByAlias(STAR_ALIAS)) {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    } else {
        return QAbstractItemModel::flags(index);
    }
}

QVariant TaskTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return {};
}

/**
 * Returns the index of the item in the model specified by the given row, column and parent index.
 * When reimplementing this function in a subclass, call createIndex() to generate model indexes that other components
 * can use to refer to items in your model.
 * @param row
 * @param column
 * @param parent
 * @return
 */
QModelIndex TaskTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    TaskTreeItem *parentItem;
    if (parent.isValid()) {
        parentItem = static_cast<TaskTreeItem *>(parent.internalPointer());
    } else {
        parentItem = rootItem;
    }

    TaskTreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return {};
}

QModelIndex TaskTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return {};
    }

    auto *childItem = static_cast<TaskTreeItem *>(index.internalPointer());
    TaskTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem) {
        return {};
    }

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

void TaskTreeModel::setModelData(QFile *file) {
    rootItem->clear();
    QVector<TaskTreeItem *> parents;
    parents << rootItem;

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(file);

//            xmlReader.raiseError(QObject::tr("Incorrect file."));

    QVector<QVector<QVariant>> dataList;
    bool rootItemInit = false;

    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == tr("TODOLIST")) {
                nextUniqueId = xmlReader.attributes().value("NEXTUNIQUEID").toInt() - 1;
            }

            if (xmlReader.name() == tr(TASK_ALIAS)) {
                int id = xmlReader.attributes().value(ID_ALIAS).toInt();
                QString title = xmlReader.attributes().value(TITLE_ALIAS).toString();
                int parentId = xmlReader.attributes().value(PARENT_ID_ALIAS).toInt();
                int priority = xmlReader.attributes().value(PRIORITY_ALIAS).toInt();
                int star = xmlReader.attributes().value(STAR_ALIAS).toInt();

                QVector<QVariant> newData(QVector<QVariant>(ColumnsData::getColumns().size()));
                const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(ID_ALIAS))).setValue(id);
                const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(TITLE_ALIAS))).setValue(title);
                const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(PARENT_ID_ALIAS))).setValue(parentId);
                const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(DEPTH_ALIAS))).setValue(0);
                const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(PRIORITY_ALIAS))).setValue(priority);
                const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(STAR_ALIAS))).setValue(star);

                if (parentId) {
                    // Find parent
                    for (const auto &i: dataList) {
                        auto &data = const_cast<QVector<QVariant> &>(i);
                        int currentId = data.at(ColumnsData::getIndexByAlias(ID_ALIAS)).value<int>();
                        int parentDepth = data.at(ColumnsData::getIndexByAlias(DEPTH_ALIAS)).value<int>();
                        if (currentId == parentId) {
                            auto &depth = const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(DEPTH_ALIAS)));
                            depth.setValue(++parentDepth);
                        }
                    }
                }

                // Add new data in collection
                dataList << newData;
            }

            if (xmlReader.name() == tr(COMMENTS_ALIAS)) {
                const QString &commentsValue = xmlReader.readElementText();
                QVector<QVariant> &last = dataList.last();
                auto &commentsVariant = const_cast<QVariant &>(last.at(ColumnsData::getIndexByAlias(COMMENTS_ALIAS)));
                commentsVariant.setValue(commentsValue);
            }
        }
    }

    for (const auto &i: dataList) {
        auto &data = const_cast<QVector<QVariant> &>(i);

        if (!rootItemInit) {
            rootItemInit = true;
            auto *newItem = new TaskTreeItem(data, parents.last());
            parents.last()->appendChild(newItem);
            parents << newItem;
        } else {
            int depth = data.at(ColumnsData::getIndexByAlias(DEPTH_ALIAS)).value<int>();
            int parentId = data.at(ColumnsData::getIndexByAlias(PARENT_ID_ALIAS)).value<int>();

            TaskTreeItem *parentItem = nullptr;
            for (auto parent: parents) {
                auto *&pParent = const_cast<TaskTreeItem *&>(parent);
                if ((depth - 1) == pParent->data(ColumnsData::getIndexByAlias(DEPTH_ALIAS)).value<int>()
                    && pParent->data(ColumnsData::getIndexByAlias(ID_ALIAS)).value<int>() == parentId) {
                    parentItem = pParent;
                }
            }

            if (parentItem == nullptr) {
                parentItem = const_cast<TaskTreeItem *&>(parents.at(depth));
            }

            auto *newItem = new TaskTreeItem(data, parentItem);
            parentItem->appendChild(newItem);
            parents << newItem;
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "Error read file:" << xmlReader.errorString();
    }

    xmlReader.clear();
    file->close();
}

bool TaskTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole) {
        auto *pItem = static_cast<TaskTreeItem *>(index.internalPointer());
        QVariant &column = const_cast<QVariant &>(pItem->getItemData().at(index.column()));
        column.setValue(value);
        return true;
    }

    return {};
}

TaskTreeItem *TaskTreeModel::getRootItem() {
    return rootItem;
}

void TaskTreeModel::deleteTask(int row, const QModelIndex &parent) {
    auto *item = static_cast<TaskTreeItem *>(parent.internalPointer());


    // Remove children
    beginRemoveRows(parent, item->childCount(), item->childCount() - 1);
    item->removeChildren();
    endRemoveRows();

    // Remove current task
    auto *parentItem = item->parentItem();
    const QModelIndex &parentIndex = indexFromItem(parentItem);
    beginRemoveRows(parentIndex, parentItem->childCount(), parentItem->childCount());
    parentItem->removeChildAtRow(row);
    endRemoveRows();
}

TaskTreeItem *TaskTreeModel::insertTask([[maybe_unused]] int row, bool isSubTask, const QModelIndex &parent) {
    auto *itemForAttach = static_cast<TaskTreeItem *>(parent.internalPointer());
    if (!isSubTask) {
        itemForAttach = itemForAttach->parentItem();
    }

    QVector<QVariant> newData(QVector<QVariant>(ColumnsData::getColumns().size()));
    const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(ID_ALIAS))).setValue(++nextUniqueId);
    const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(TITLE_ALIAS))).setValue(QString("Task"));
    const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(PARENT_ID_ALIAS))).setValue(itemForAttach->getId());
    const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(DEPTH_ALIAS))).setValue(0);
    const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(PRIORITY_ALIAS))).setValue(DEFAULT_PRIORITY);
    const_cast<QVariant &>(newData.at(ColumnsData::getIndexByAlias(COMMENTS_ALIAS))).setValue(
            "New task #" + QString::number(nextUniqueId));

    auto *newTaskItem = new TaskTreeItem(newData, itemForAttach);

    if (isSubTask) {
        beginInsertRows(parent, 0, 0);
        itemForAttach->appendChild(newTaskItem);
    } else {
        beginInsertRows(indexFromItem(itemForAttach), itemForAttach->childCount(), itemForAttach->childCount() - 1);
        itemForAttach->appendChildAtRow(row + 1, newTaskItem);
    }

    endInsertRows();

    return newTaskItem;
}

QModelIndex TaskTreeModel::indexFromItem(TaskTreeItem *item) {
    if (item == nullptr || item == rootItem) {
        return {};
    }

    TaskTreeItem *parent = item->parentItem();
    QList<TaskTreeItem *> parents;
    while (parent && parent != rootItem) {
        parents << parent;
        parent = parent->parentItem();
    }

    QModelIndex ix;

    for (int i = 0; i < parents.count(); i++) {
        ix = index(parents[i]->row(), 0, ix);
    }
    ix = index(ix.row(), 0, ix);

    return ix;
}
