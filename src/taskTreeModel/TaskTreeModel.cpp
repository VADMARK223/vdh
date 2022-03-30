//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"

#define ID_INDEX 0
#define PARENT_ID_INDEX 1
#define DEPTH_INDEX 2

TaskTreeModel::TaskTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    rootItem = new TaskTreeItem({tr("ID"), tr("Parent ID"), tr("Depth")});
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
    if (!index.isValid()) {
        return {};
    }

    if (role != Qt::DisplayRole) {
        return {};
    }

    auto *item = static_cast<TaskTreeItem *>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TaskTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TaskTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return {};
}

QModelIndex TaskTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    TaskTreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TaskTreeItem *>(parent.internalPointer());
    }

    TaskTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
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

void TaskTreeModel::setModelDate(QFile *file) {
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
            if (xmlReader.name() == tr("TASK")) {
                int id = xmlReader.attributes().value("ID").toInt();
                int parentId = xmlReader.attributes().value("P").toInt();
                QVector<QVariant> newData;
                newData << QList<QVariant>({QVariant(id), QVariant(parentId), QVariant(0)});

                if (parentId) {
                    // Find parent
                    for (const auto &i: dataList) {
                        auto &data = const_cast<QVector<QVariant> &>(i);
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
        auto &data = const_cast<QVector<QVariant> &>(i);

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

    if (xmlReader.hasError()) {
        qDebug() << "Error read file:" << xmlReader.errorString();
    }

    xmlReader.clear();
    file->close();
}
