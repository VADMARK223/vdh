//
// Created by v.markitanov on 29.03.2022.
//

#include "MainWindow.h"
#include "ColumnsData.h"
#include <QMenuBar>
#include <QFile>
#include <QLabel>
#include <QSplitter>
#include <QMessageBox>
#include <QDir>
#include <QToolBar>
#include <QPushButton>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
//    loadSettings();

    setMenuBar(createMenuBar());
    addToolBar(createToolBar());

    auto *progressBar = new QProgressBar;
    progressBar->setValue(4);
    _statusBar->addPermanentWidget(progressBar);
    setStatusBar(_statusBar);

    _treeView->setModel(_model);
    _treeView->setHeaderHidden(false);
    _treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    _treeView->setSortingEnabled(false);


    loadSettings();
/*#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
    if (AUTO_LOAD_MODEL) { // NOLINT
        QString filePath(":files/data.xml");
        loadModelFromByFilePath(filePath);
    }
#pragma clang diagnostic pop*/

    QPixmap pixmap(":images/logo.png");
    auto *label = new QLabel;
    label->setPixmap(pixmap);

    auto *closeButton = new QPushButton("Close");

    auto *splitter = new QSplitter;
    splitter->addWidget(_treeView);
    splitter->addWidget(label);
    splitter->addWidget(closeButton);
    splitter->show();

    _treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    for (const auto &column: ColumnsData::getColumns()) {
        _treeView->setColumnWidth(column.index, column.width);
        _treeView->setColumnHidden(column.index, column.hide);
    }

    closeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(splitter);

    connect(_treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this,
            SLOT(onSelectionChanged(QItemSelection, QItemSelection)));

    connect(_treeView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(onCurrentChanged(QModelIndex, QModelIndex)));

    connect(_treeView->model(),
            SIGNAL(dataChanged(QModelIndex, QModelIndex, QList<int>)),
            this,
            SLOT(onDataChanged(QModelIndex, QModelIndex, QList<int>)));

    connect(_treeView->model(),
            SIGNAL(rowsInserted(QModelIndex, int, int)),
            this,
            SLOT(onRowsInserted(QModelIndex, int, int)));

    connect(_treeView->model(),
            SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)),
            this,
            SLOT(onRowsAboutToBeInserted(QModelIndex, int, int)));


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
    if (_file == nullptr) {
        return;
    }

    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + _file->fileName() + "'.", QMessageBox::Ok);
        _file->close();
        return;
    }

    QXmlStreamWriter writer;
    writer.setDevice(_file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("TODOLIST");
    writer.writeAttribute("NEXTUNIQUEID", QString::number(_model->nextUniqueId + 1));
    writeElement(writer, _model->getRootItem());
    writer.writeEndDocument();
    _file->close();
}

void MainWindow::onSaveFileAs() {
    QString filter = "Tasklist (*.xml)";
    QString filePath = QFileDialog::getSaveFileName(this, "Save Tasklist As", QDir::homePath(), filter);

    if (filePath.isEmpty()) {
        return;
    }

    _file->setFileName(filePath);

    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        _file->close();
        return;
    }

    QXmlStreamWriter writer;
    writer.setDevice(_file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("TODOLIST");
    writer.writeAttribute("NEXTUNIQUEID", QString::number(_model->nextUniqueId));
    writeElement(writer, _model->getRootItem());
    writer.writeEndDocument();
    _file->close();
}

void MainWindow::addTaskAction(bool isSubtask) {
    bool hasSelection = _treeView->selectionModel()->hasSelection();
    if (!hasSelection) {
        QMessageBox::information(this, "Info", "Please select task.", QMessageBox::Ok);
        return;
    }
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();
    auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
    /*TaskTreeItem *newTaskItem =*/ _model->insertTask(selectedIndex.row(), isSubtask, selectedIndex);

    if (isSubtask) {
        _treeView->expand(selectedIndex);
    }

//    QItemSelection selection = _treeView->selectionModel()->selection();
    // Unselected old task
//    const QModelIndex &topLeft = _model->index(newTaskItem->row(), ID_INDEX, selectedIndex);
//    const QModelIndex &bottomRight = _model->index(newTaskItem->row(), COMMENTS_INDEX, selectedIndex);
//    selection.select(topLeft, bottomRight);
//    _treeView->selectionModel()->select(selection, QItemSelectionModel::Deselect);

    // Selected new task
//    const QModelIndex &parent = isSubtask ? selectedIndex : QModelIndex();
//    selection.select(_model->index(newTaskItem->row(), ID_INDEX, parent),
//                     _model->index(newTaskItem->row(), COMMENTS_INDEX, parent));
//    _treeView->selectionModel()->select(selection, QItemSelectionModel::Select);
}

void MainWindow::loadModelFromByFilePath(const QString &filePath) {
    if (filePath.isEmpty()) {
        return;
    }

    _statusBar->showMessage(filePath);

    _file->setFileName(filePath);

    if (!_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        _file->close();
        return;
    }

//    _treeView->reset();
    _model->setModelData(_file);
    _treeView->expandAll();

    if (!_treeView->selectionModel()->hasSelection()) {
        // Selected first row
        const QModelIndex &parent = QModelIndex();
        const QModelIndex &topLeft = _model->index(0, 0, parent);
        const QModelIndex &bottomRight = _model->index(0, static_cast<int>(ColumnsData::getColumns().size()) - 1,
                                                       parent);
        QItemSelection selection = _treeView->selectionModel()->selection();
        selection.select(topLeft, bottomRight);
        _treeView->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
    }

    _file->close();
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

    auto *toolsMenu = new QMenu("&Tools");
    toolsMenu->addAction("Preferences...");

    auto *helpMenu = new QMenu("&Help");
    helpMenu->addAction("&About vdh");

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(toolsMenu);
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

void MainWindow::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                               const QList<int> &roles) {
    qDebug() << "onDataChanged.";
}

void MainWindow::onRowsInserted(const QModelIndex &parent, int first, int last) {
//    auto *pTreeItem = static_cast<TaskTreeItem *>(parent.internalPointer());
//    qDebug() << "Insert pTreeItem:" << pTreeItem->toString();
}

void MainWindow::onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last) {
//    auto *pTreeItem = static_cast<TaskTreeItem *>(parent.internalPointer());
//    qDebug() << "Insert pTreeItem:" << pTreeItem->toString();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();
}

void MainWindow::loadSettings() {
    _settings->beginGroup("MainWindowPosition");
    int x = _settings->value("x", -1).toInt();
    int y = _settings->value("y", -1).toInt();
    int width = _settings->value("width", -1).toInt();
    int height = _settings->value("height", -1).toInt();
    _settings->endGroup();

    _settings->beginGroup("Preferences");
    const QString &fileName = _settings->value("fileName", "").toString();
    if (!fileName.isEmpty()) {
        loadModelFromByFilePath(fileName);
    }
    _settings->endGroup();

    this->setMinimumSize(1000, 500);
    if (x > 0 && y > 0 && width > 0 && height > 0) {
        this->setGeometry(x, y, width, height);
    }
}

void MainWindow::writeSettings() {
    _settings->beginGroup("MainWindowPosition");
    _settings->setValue("x", this->x());
    _settings->setValue("y", this->y());
    _settings->setValue("width", this->width());
    _settings->setValue("height", this->height());
    _settings->endGroup();

    _settings->beginGroup("Preferences");
    _settings->setValue("fileName", _file->fileName());
    _settings->endGroup();
}
