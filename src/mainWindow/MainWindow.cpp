//
// Created by v.markitanov on 29.03.2022.
//

#include <QFile>
#include <QTreeView>
#include <QLabel>
#include <QSplitter>
#include "MainWindow.h"
#include "../taskTreeModel/TaskTreeModel.h"

MainWindow::MainWindow(QWidget *parent) {

    QFile file(":files/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error open file!";
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
    splitter->show();

    setCentralWidget(splitter);
}
