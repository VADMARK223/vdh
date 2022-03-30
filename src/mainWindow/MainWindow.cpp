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
#include <QMessageBox>
#include <QXmlStreamReader>

MainWindow::MainWindow(QWidget *parent) {
    auto *menuBar = new QMenuBar;

    auto *fileMenu = new QMenu("&File");
    fileMenu->addAction("&Open Tasklist...", this, SLOT(onOpenFileClicked()), tr("Ctrl+O"));
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, SLOT(close()), tr("Alt+F4"));
    menuBar->addMenu(fileMenu);
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
    QString filePath(":files/data.xml");
    auto *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        file->close();
        return {};
    }

    auto *model = new TaskTreeModel();
    model->setModelDate(file);
    file->close();
    return model;
}

void MainWindow::onOpenFileClicked() {
    QString filePath(":files/data.xml");
    auto *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
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
            if (xmlReader.name() == tr("TODOLIST")) {
                continue;
            }

            if (xmlReader.name() == tr("TASK")) {
                qDebug() << xmlReader.readElementText();
            }
        }
    }

    if (xmlReader.hasError()) {
        QMessageBox::critical(this, "Error read file", xmlReader.errorString(), QMessageBox::Ok);
    }

    xmlReader.clear();
    file->close();
}
