#include <QApplication>
#include <QTreeView>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QSplitter>
#include <QFile>
#include "mainWindow.h"
#include "src/taskTreeModel/TaskTreeModel.h"

#define F_NAME "objectName"
#define F_AGE "age"
#define F_SPENT "spent"
#define F_DESC "desc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    MainWindow mainWindow;
//    mainWindow.show();

//    Q_INIT_RESOURCE(vdh);

    QFile file("data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error open file!";
        return 0;
    }

    auto *model = new TaskTreeModel(file.readAll());
    file.close();

    auto *treeView = new QTreeView();
    treeView->setHeaderHidden(false);
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
    splitter->setSizes(QList<int>({200, 100}));
    splitter->show();

    return QApplication::exec();
}
