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
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setMenuBar(createMenuBar());
    addToolBar(createToolBar());

    _treeView->setHeaderHidden(false);
    _treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _treeView->setSortingEnabled(false);
    _treeView->setSelectionBehavior(QAbstractItemView::SelectRows);


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

    auto *closeButton = new QPushButton("Close");

    auto *splitter = new QSplitter;
    splitter->addWidget(_treeView);
    splitter->addWidget(label);
    splitter->addWidget(closeButton);
//    splitter->setSizes(QList<int>({200, 100}));
    splitter->show();

    setCentralWidget(splitter);

    connect(_treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,
            SLOT(onSelectionChanged(QItemSelection, QItemSelection)));

    connect(_treeView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(onCurrentChanged(QModelIndex, QModelIndex)));


    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));
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

void MainWindow::addTaskAction(bool isSubtask) {
    bool hasSelection = _treeView->selectionModel()->hasSelection();
    if (!hasSelection) {
        QMessageBox::information(this, "Info", "Please select task.", QMessageBox::Ok);
        return;
    }
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();

    if (isSubtask) {
        auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
        _model->insertTask(selectedIndex.row(), isSubtask, selectedIndex);
    } else {
        auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
        _model->insertTask(selectedIndex.row(), isSubtask, selectedIndex);
        /*const QModelIndex &selectedIndex = selectedIndexesList.first();
        TaskTreeItem *pItem = _model->getRootItem();
        qDebug() << "pItem:" << pItem;
        qDebug() << "selected Index:" << selectedIndex;

        const QModelIndex &resultIndex = _model->index(0, 0, QModelIndex());
        qDebug() << "resultIndex:" << resultIndex;
        _model->insertTask(selectedIndex.row(), isSubtask, selectedIndex);*/
    }



//    if (isSubtask) {
//        const QModelIndex &currentIndex = _treeView->selectionModel()->currentIndex();
//        qDebug() << "Current index:" << currentIndex;
//
//        bool isCurrentIndexSelected = _treeView->selectionModel()->isSelected(currentIndex);
//        qDebug() << "Is current index selected:" << isCurrentIndexSelected;
//
//        const QItemSelection &selection = _treeView->selectionModel()->selection();
//        qDebug() << "Selection:" << selection;
//

//        qDebug() << "Has selection:" << hasSelection;
//
//        if (selectedIndexesList.isEmpty()) {
//            _model->insertSubtask(0);
//        } else {
//            auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
//            _model->insertSubtask(selectedIndex.row(), selectedIndex);
//        }
//    } else {
//        if (selectedIndexesList.isEmpty()) {
//            _model->insertRow(0);
//        } else {
//            auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
//            _model->insertRow(selectedIndex.row(), selectedIndex);
//        }
}


//void MainWindow::addSubTaskAction() {
//    const QModelIndex &index = _treeView->currentIndex().sibling(1,1);
//    qDebug() << "index:" << index;
//    _treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);


//    const QModelIndex &modelIndex = _model->index(0,0, nullptr);//QModelIndex(0,0,);
//    qDebug() << "modelIndex:" << modelIndex;
//    _treeView->selectionModel()->select(modelIndex, QItemSelectionModel::ClearAndSelect);
//}

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
    toolBar->addAction(QPixmap(":images/task-add-20.png"), "New Task", this, [this] { addTaskAction(false); });
//    QAction *pAction = toolBar->addAction(QPixmap(":images/task-add-20.png"), "New Task");
//    connect(pAction, &QAction::triggered, this, [this] { addTaskAction(); });

    toolBar->addAction(QPixmap(":images/sub-task-add.png"), "New Subtask", this, [this] { addTaskAction(true); });
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

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
//    qDebug() << "Selection change selected:" << selected << "deselected:" << deselected;
}

void MainWindow::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous) {
//    qDebug() << "Current change:" << current;
}