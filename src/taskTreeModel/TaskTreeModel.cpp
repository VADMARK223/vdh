//
// Created by v.markitanov on 28.03.2022.
//

#include "TaskTreeModel.h"

#include <QStringList>
#include <QXmlStreamReader>
#include <QMessageBox>

TaskTreeModel::TaskTreeModel(QFile *file, QObject *parent)
        : QAbstractItemModel(parent) {
    QByteArray data = file->readAll();
    qDebug() << "Data: " << data;
    rootItem = new TaskTreeItem({tr("Title"), tr("ID"), tr("Description")});
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

    while (!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == tr("TASK")) {
                const QString &title = xmlReader.attributes().value("TITLE").toString();
                const QString &id = xmlReader.attributes().value("ID").toString();
                QVector<QVariant> task;
                task << QList<QVariant>({QVariant(title), QVariant(id)});

                qDebug() << title << " Size: " << parents.size() << " Count: " << parents.count();

                auto *pItem = new TaskTreeItem(task, parents.last());
                parents.last()->appendChild(pItem);
                parents << pItem;
                parents.pop_back();

                /*if (isSubTask) {
                    TaskTreeItem *pItem = parents.last()->child(0);
                    qDebug() << "1";
                    pItem->appendChild(new TaskTreeItem(task, pItem));
                    parents << pItem;
                } else {
                    TaskTreeItem *pItem = parents.last()->child(0);
                    if (pItem == nullptr) {
                        qDebug() << "Add root";
                        parents.last()->appendChild(new TaskTreeItem(task, parents.last()));
                    } else {
                        qDebug() << "2";
                        pItem->appendChild(new TaskTreeItem(task, pItem));
                        parents.pop_back();
                    }
                }*/
            }
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "ERROR";
        //QMessageBox::critical(this, "Error read file", xmlReader.errorString(), QMessageBox::Ok);
    }

    xmlReader.clear();
    file->close();

    /*QVector<QVariant> task1;
    task1 << QList<QVariant>({QVariant("Task 1"), QVariant("1")});
    parents.last()->appendChild(new TaskTreeItem(task1, parents.last()));

    QVector<QVariant> subTask2;
    subTask2 << QList<QVariant>({QVariant("SubTask 1"), QVariant("2")});
    TaskTreeItem *pItem = parents.last()->child(0);
    pItem->appendChild(new TaskTreeItem(subTask2, pItem));

    QVector<QVariant> task2;
    task2 << QList<QVariant>({QVariant("Task 2"), QVariant("3")});
    rootItem->appendChild(new TaskTreeItem(task2, rootItem));*/

    /*
    QVector<int> indentations;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            const QStringList columnStrings =
                    lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);

            QVector<QVariant> columnData;
            columnData.reserve(columnStrings.count());


            for (const QString &columnString: columnStrings)
                columnData << columnString;

            if (position > indentations.last()) {
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

            parents.last()->appendChild(new TaskTreeItem(columnData, parents.last()));
        }
        ++number;
    }*/
}
