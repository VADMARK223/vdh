//
// Created by v.markitanov on 29.03.2022.
//

#include "MainWindow.h"
#include <QMenuBar>
#include <QFile>
#include <QLabel>
#include <QSplitter>
#include <QShortcut>
#include <QMessageBox>
#include <QDir>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setMenuBar(createMenuBar());
    addToolBar(createToolBar());

    _treeView->setHeaderHidden(false);
    _treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _treeView->setSortingEnabled(false);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
    if (AUTO_LOAD_MODEL) { // NOLINT
        QString filePath(":files/data.xml");
        loadModelFromByFilePath(filePath);
    }
#pragma clang diagnostic pop

    QPixmap pixmap(":images/logo.png");
    auto *label = new QLabel;
    label->setPixmap(pixmap);

    auto *splitter = new QSplitter;
    splitter->addWidget(_treeView);
    splitter->addWidget(label);
    splitter->setSizes(QList<int>({200, 100}));
    splitter->show();

    setCentralWidget(splitter);

    connect(_treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,
            SLOT(treeViewSelectionChange(QItemSelection, QItemSelection)));
}

void MainWindow::newFileAction() {
    qDebug() << "New file action.";
}

void MainWindow::preferenceAction() {
    qDebug() << "Preference action:";
}

void MainWindow::onOpenFileClicked() {
    QString filter = "Tasklist (*.xml)";
    QString filePath = QFileDialog::getOpenFileName(this, "Open Tasklist", QDir::homePath(), filter);
    loadModelFromByFilePath(filePath);
}

void MainWindow::saveAction() {
    qDebug() << "Save";
}

void MainWindow::onSaveFileAs() {
    QString filter = "Tasklist (*.xml)";
    QString filePath = QFileDialog::getSaveFileName(this, "Save Tasklist As", QDir::homePath(), filter);

    if (filePath.isEmpty()) {
        return;
    }

    QFile qFile(filePath);
    if (!qFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        qFile.close();
        return;
    }

    QXmlStreamWriter writer;
    writer.setDevice(&qFile);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("TODOLIST");
    writer.writeAttribute("NEXTUNIQUEID", QString::number(_model->nextUniqueId));
    writeElement(writer, _model->getRootItem());
    writer.writeEndDocument();
    qFile.close();
}

void MainWindow::addTaskAction() {
    qDebug() << "Add task action.";
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();
    qDebug() << "Selected indexes:" << selectedIndexesList;
    if (selectedIndexesList.isEmpty()) {
        qDebug() << "Add in end.";
        _model->insertRow(0);
    } else {
        auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
        _model->insertRow(selectedIndex.row(), selectedIndex);
    }
}

void MainWindow::addSubTaskAction() {
    qDebug() << "Add sub task action.";
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();
    qDebug() << "Selected indexes:" << selectedIndexesList;
    if (selectedIndexesList.isEmpty()) {
        qDebug() << "Add in end.";
        _model->insertSubtask(0);
    } else {
        auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
        _model->insertSubtask(selectedIndex.row(), selectedIndex);
    }
}

void MainWindow::loadModelFromByFilePath(const QString &filePath) {
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        file.close();
        return;
    }


    _model->setModelData(&file);
    setModel(_model);

    file.close();
}

void MainWindow::setModel(TaskTreeModel *pModel) {
    _treeView->setModel(pModel);
}

QMenuBar *MainWindow::createMenuBar() {
    auto *menuBar = new QMenuBar;

    auto *fileMenu = new QMenu("&File");
    fileMenu->addAction("&New Tasklist...", this, SLOT(newFileAction()));
    fileMenu->addSeparator();
    fileMenu->addAction("&Open Tasklist...", this, SLOT(onOpenFileClicked()), tr("Ctrl+O"));
    fileMenu->addAction("Save Tasklist &As...", this, SLOT(onSaveFileAs()));
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, SLOT(close()), tr("Alt+F4"));

    auto *helpMenu = new QMenu("&Help");
    helpMenu->addAction("&About vdh");

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(helpMenu);

    return menuBar;
}

QToolBar *MainWindow::createToolBar() {
    auto *toolBar = new QToolBar();
    toolBar->addAction(QPixmap(":images/file-open-20.png"), "Open Tasklist (Ctrl+O)", this, SLOT(onOpenFileClicked()));
    toolBar->addAction(QPixmap(":images/tasklist-save-20.png"), "Save Tasklist", this, SLOT(saveAction()));
    toolBar->addSeparator();
    toolBar->addAction(QPixmap(":images/task-add-20.png"), "New Task", this, SLOT(addTaskAction()));
    toolBar->addAction(QPixmap(":images/sub-task-add.png"), "New Subtask", this, SLOT(addSubTaskAction()));
    toolBar->addSeparator();
    toolBar->addAction(QPixmap(":images/tasklist-new.png"), "New Tasklist", this, SLOT(newFileAction()));
    toolBar->addAction(QPixmap(":images/settings.png"), "Preference", this, SLOT(preferenceAction()));
    addToolBar(toolBar);
    return toolBar;
}

void MainWindow::writeElement(QXmlStreamWriter &writer, TaskTreeItem *root) {
    for (int i = 0; i < root->childCount(); ++i) {
        TaskTreeItem *child = root->child(i);
        writer.writeStartElement("TASK");
        writer.writeAttribute("ID", QString::number(child->getId()));
        writer.writeAttribute("P", QString::number(child->getParentId()));

        if (child->childCount()) {
            writeElement(writer, child);
        }

        writer.writeEndElement();
    }
}

void MainWindow::treeViewSelectionChange(const QItemSelection &selected, const QItemSelection &deselected) {
    qDebug() << "Selection change:" << selected;

}
