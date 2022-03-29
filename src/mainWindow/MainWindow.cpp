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
    QString filePath(":files/data.txt");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        file.close();
        return {};
    }

    auto *model = new TaskTreeModel(file.readAll());
    file.close();
    return model;
}

void MainWindow::onOpenFileClicked() {
    QString filePath(":files/data.xml");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        file.close();
    }

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd()) {
        qDebug() << "xmlReader.readElementText()";
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        //If token is just StartDocument - go to next
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }
        //If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == tr("tasks")) {
                continue;
            }

            if (xmlReader.name() == tr("task")) {
                qDebug() << xmlReader.readElementText();
            }
        }
    }

    if (xmlReader.hasError()) {
        QMessageBox::critical(this, "Error read file", xmlReader.errorString(), QMessageBox::Ok);
    }

    xmlReader.clear();
    file.close();
}
