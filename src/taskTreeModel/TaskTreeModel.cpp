//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"

#include <QStringList>
#include <QXmlStreamReader>
#include <QMessageBox>

#define ID_INDEX 0
#define PARENT_ID_INDEX 1
#define DEPTH_INDEX 2

TaskTreeModel::TaskTreeModel(QFile *file, QObject *parent)
        : QAbstractItemModel(parent) {
    QByteArray data = file->readAll();
    qDebug() << "Data: " << data;
    rootItem = new TaskTreeItem({/*tr("Title"),*/ tr("ID"), tr("Parent ID"), tr("Depth")});
//    setupModelData(data.split('\n'), rootItem);
    setupModelData(file, rootItem);
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

void TaskTreeModel::setupModelData(QFile *file1, TaskTreeItem *parent) {
    QVector<TaskTreeItem *> parents;
    parents << parent;

    QString filePath(":files/data.xml");
    auto *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        file->close();
    }

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
                    for (int i = 0; i < dataList.size(); ++i) {
                        auto &data = const_cast<QVector<QVariant> &>(dataList.at(i));
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

    for (int i = 0; i < dataList.size(); ++i) {
        auto &data = const_cast<QVector<QVariant> &>(dataList.at(i));

        if (!rootItemInit) {
            rootItemInit = true;
            auto *newItem = new TaskTreeItem(data, parents.last());
            parents.last()->appendChild(newItem);
            parents << newItem;
        } else {
            int depth = data.at(DEPTH_INDEX).value<int>();
            int parentId = data.at(PARENT_ID_INDEX).value<int>();

            TaskTreeItem *parentItem = nullptr;
            for (int j = 0; j < parents.size(); ++j) {
                auto *&pItem = const_cast<TaskTreeItem *&>(parents.at(j));
                int tempId = pItem->data(ID_INDEX).value<int>();
                int tempDepth = pItem->data(DEPTH_INDEX).value<int>();
                if ((depth - 1) == tempDepth && tempId == parentId) {
                    parentItem = pItem;
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
