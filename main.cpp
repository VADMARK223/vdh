#include <QApplication>
#include <QTreeView>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QSplitter>
#include <QStandardItemModel>
#include "mainWindow.h"
#include "objectTreeModel.h"

#define F_NAME "objectName"
#define F_AGE "age"
#define F_SPENT "spent"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    MainWindow mainWindow;
//    mainWindow.show();

    auto *model = new ObjectTreeModel();
    QStringList columns;
    columns << F_NAME << F_AGE << F_SPENT;
    model->setColumns(columns);

    auto *item1 = new QObject();
    item1->setObjectName("March");
    item1->setProperty(F_AGE, 45);

    auto *item2 = new QObject(item1);
    item2->setProperty(F_NAME, "Week 4");
    item2->setProperty(F_AGE, 14);

    auto *item3 = new QObject(item2);
    item3->setProperty(F_NAME, "27");
    item3->setProperty(F_AGE, 14);

    auto *item4 = new QObject(item3);
    item4->setProperty(F_NAME, "Task #1");
    item4->setProperty(F_AGE, 14);
    item4->setProperty(F_SPENT, "15m");

    model->addItem(item1, QModelIndex());



    auto *treeView = new QTreeView();
    treeView->setHeaderHidden(false);
//    QStandardItemModel * temp = new QStandardItemModel;
//    temp->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, "Col", Qt::EditRole);
//    treeView->setModel(temp);
    treeView->setModel(model);

    auto *child1 = new QTreeWidgetItem;
    child1->setText(0, "Child1");
    child1->setText(1, "Description1");

    auto *child2 = new QTreeWidgetItem;
    child2->setText(0, "Child2");
    child2->setText(1, "Description2");

    auto *topLevelItem = new QTreeWidgetItem;
    topLevelItem->setText(0, "Name");
    topLevelItem->setText(1, "Description");
    topLevelItem->addChild(child1);
    topLevelItem->addChild(child2);

    auto *treeWidget = new QTreeWidget();
    QStringList treeWidgetHeaderLabels;
    treeWidgetHeaderLabels << "Column #1" << "Column #2";
    treeWidget->setHeaderLabels(treeWidgetHeaderLabels);
    treeWidget->addTopLevelItem(topLevelItem);
    treeWidget->setColumnCount(2);

    auto *splitter = new QSplitter;
    splitter->addWidget(treeView);
    splitter->addWidget(treeWidget);
    splitter->show();

    return QApplication::exec();
}
