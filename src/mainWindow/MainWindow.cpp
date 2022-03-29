//
// Created by v.markitanov on 29.03.2022.
//

#include "MainWindow.h"
#include <QMenuBar>
#include <QFile>
#include <QTreeView>
#include <QLabel>
#include <QSplitter>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) {
    auto *menuBar = new QMenuBar;
    auto *menu = new QMenu("&File");
    menu->addAction("E&xit", this, SLOT(close()), tr("Alt+F4"));
    menu->addSeparator();

    menuBar->addMenu(menu);
    setMenuBar(menuBar);

    auto *model = createModel();

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

TaskTreeModel *MainWindow::createModel() {
    QFile file(":files/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error open file!";
    }

    auto *model = new TaskTreeModel(file.readAll());
    file.close();
    return model;
}
