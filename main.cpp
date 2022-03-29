#include <QApplication>
#include <QTreeView>
#include <QHeaderView>
#include <QSplitter>
#include <QFile>
#include <QLabel>
#include <QLayout>
#include "mainWindowOld.h"
#include "src/taskTreeModel/TaskTreeModel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindowOld mainWindow;
    mainWindow.show();

    /*QFile file(":files/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error open file!";
        return 0;
    }

    auto *model = new TaskTreeModel(file.readAll());
    file.close();

    auto *treeView = new QTreeView();
    treeView->setHeaderHidden(false);
    treeView->setModel(model);

    QPixmap pixmap(":images/logo.png");
    auto *label = new QLabel;
    label->setPixmap(pixmap);

    auto *splitter = new QSplitter;
    splitter->addWidget(treeView);
    splitter->addWidget(label);
    splitter->setSizes(QList<int>({200, 100}));
    splitter->show();*/

    return QApplication::exec();
}
