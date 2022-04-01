//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"


TaskTreeModel::TaskTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    rootItem = new TaskTreeItem({
                                        tr("ID"),
                                        tr("Parent ID"),
                                        tr("Depth"),
                                        tr("Comments")
                                });
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

        case Qt::BackgroundRole: {
            auto *item = static_cast<TaskTreeItem *>(index.internalPointer());
            int depth = item->data(DEPTH_INDEX).value<int>();
            if (depth >= 2) {
                return QBrush(Qt::red);
            }
        }

        default:
//            qDebug() << "Role:" << role;
            return {};
    }
}

Qt::ItemFlags TaskTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

//    auto *item = static_cast<TaskTreeItem *>(index.internalPointer());
//    if (item->getId() == 2) {

//    index.column() == ID_INDEX

    if (index.column() == COMMENTS_INDEX) {
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
    QVector < TaskTreeItem * > parents;
    parents << rootItem;

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(file);

//            xmlReader.raiseError(QObject::tr("Incorrect file."));

    QVector <QVector<QVariant>> dataList;
    bool rootItemInit = false;

    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == tr("TODOLIST")) {
                nextUniqueId = xmlReader.attributes().value("NEXTUNIQUEID").toInt();
                qDebug() << "TODOLIST:" << nextUniqueId;
            }

            if (xmlReader.name() == tr("TASK")) {
                int id = xmlReader.attributes().value("ID").toInt();
                int parentId = xmlReader.attributes().value("P").toInt();
                QVector <QVariant> newData;
                newData << QList<QVariant>({QVariant(id), QVariant(parentId), QVariant(0), QVariant("Comment")});

                if (parentId) {
                    // Find parent
                    for (const auto &i: dataList) {
                        auto &data = const_cast<QVector <QVariant> &>(i);
                        int currentId = data.at(ID_INDEX).value<int>();
                        int parentDepth = data.at(DEPTH_INDEX).value<int>();
                        if (currentId == parentId) {
                            auto &depth = const_cast<QVariant &>(newData.at(DEPTH_INDEX));
                            depth.setValue(++parentDepth);
                        }
                    }
                }

                // Add new data in collection
                dataList << newData;
            }
        }
    }

    for (const auto &i: dataList) {
        auto &data = const_cast<QVector <QVariant> &>(i);

        if (!rootItemInit) {
            rootItemInit = true;
            auto *newItem = new TaskTreeItem(data, parents.last());
            parents.last()->appendChild(newItem);
            parents << newItem;
        } else {
            int depth = data.at(DEPTH_INDEX).value<int>();
            int parentId = data.at(PARENT_ID_INDEX).value<int>();

            TaskTreeItem *parentItem = nullptr;
            for (auto parent: parents) {
                auto *&pParent = const_cast<TaskTreeItem *&>(parent);
                if ((depth - 1) == pParent->data(DEPTH_INDEX).value<int>()
                    && pParent->data(ID_INDEX).value<int>() == parentId) {
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

//    QVector<QVariant> newTask;
//    newTask << QList<QVariant>({QVariant(99), QVariant(99), QVariant(98)});
//    rootItem->appendChild(new TaskTreeItem(newTask, rootItem));
//
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

bool TaskTreeModel::insertRow(int row, const QModelIndex &parent) {
    qDebug() << "Insert row:" << row << parent;
//    beginInsertRows(parent, 0, 0);
    beginInsertRows(QModelIndex(), 0, 0);

    QVector<QVariant> newTask;
    QString comment = "New comment";
    newTask << QList<QVariant>({QVariant(nextUniqueId++), QVariant(0), QVariant(0), QVariant(comment)});
    if (parent.internalPointer() == nullptr) {
//    TaskTreeItem *pItem = getRootItem()->child(0);
//        TaskTreeItem *pItem = getRootItem();
//        rootItem->appendChild(new TaskTreeItem(newTask, rootItem));
        rootItem->appendChild(new TaskTreeItem(newTask, rootItem));
    } else {
        rootItem->_childItems.insert(row + 1, new TaskTreeItem(newTask, rootItem));
//        auto *pTreeItem = static_cast<TaskTreeItem *>(parent.internalPointer());
//        pTreeItem->_childItems.insert(row + 1, new TaskTreeItem(newTask, pTreeItem));

//        auto *pTreeItem = static_cast<TaskTreeItem *>(parent.internalPointer());
//        qDebug() << "Parent item:" << pTreeItem->toString();
//        QVector<QVariant> newTask;
//        newTask << QList<QVariant>({QVariant(99), QVariant(99), QVariant(98), QVariant("Comment99")});
//        rootItem->_childItems.insert(row, new TaskTreeItem(newTask, rootItem));
//        pTreeItem->appendChild(new TaskTreeItem(newTask, pTreeItem));
    }

    endInsertRows();
    return true;
}

void TaskTreeModel::insertSubtask(int row, const QModelIndex &parent) {
    qDebug() << "Insert row:" << row << parent;
//    beginInsertRows(parent, 0, 0);
    beginInsertRows(QModelIndex(), 0, 0);

    QVector<QVariant> newTask;
    QString comment = "New comment";
    newTask << QList<QVariant>({QVariant(nextUniqueId++), QVariant(0), QVariant(0), QVariant(comment)});
    if (parent.internalPointer() == nullptr) {
        rootItem->appendChild(new TaskTreeItem(newTask, rootItem));
    } else {
        auto *pTreeItem = static_cast<TaskTreeItem *>(parent.internalPointer());
        qDebug() << "Parent item:" << pTreeItem->toString();
        pTreeItem->appendChild(new TaskTreeItem(newTask, pTreeItem));
    }

    endInsertRows();
}
