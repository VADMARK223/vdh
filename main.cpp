#include <QApplication>
#include <QTreeView>
#include "mainWindow.h"
#include "objectTreeModel.h"

#define F_NAME "objectName"
#define F_AGE "age"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    MainWindow mainWindow;
//    mainWindow.show();
//    auto *splitter = new QSplitter;

    auto *model = new ObjectTreeModel();
    QStringList columns;
    columns << F_NAME << F_AGE;
    model->setColumns(columns);

    auto *item1 = new QObject();
    item1->setObjectName("Father");
    item1->setProperty(F_AGE, 45);

    auto *item2 = new QObject(item1);
    item2->setProperty(F_NAME, "Son");
    item2->setProperty(F_AGE, 14);

    model->addItem(item1, QModelIndex());

    auto *treeView = new QTreeView();
    treeView->setModel(model);
    treeView->show();

    return QApplication::exec();
}
