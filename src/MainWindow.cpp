//
// Created by v.markitanov on 29.03.2022.
//

#include "MainWindow.h"
#include "ColumnsData.h"
#include "delegate/DepthDelegate.h"
#include "delegate/title/TitleDelegate.h"
#include "delegate/StarDelegate.h"
#include "delegate/title/TitleEditor.h"
#include <QMenuBar>
#include <QFile>
#include <QSplitter>
#include <QMessageBox>
#include <QDir>
#include <QToolBar>
#include <QPushButton>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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
    _treeView->setEditTriggers(_treeView->DoubleClicked);

    _treeView->setItemDelegateForColumn(ColumnsData::getIndexByAlias(TITLE_ALIAS), new TitleDelegate(this));

    auto *depthDelegate = new DepthDelegate();
    _treeView->setItemDelegateForColumn(ColumnsData::getIndexByAlias(DEPTH_ALIAS), depthDelegate);

    auto *starDelegate = new StarDelegate();
    _treeView->setItemDelegateForColumn(ColumnsData::getIndexByAlias(STAR_ALIAS), starDelegate);

    _commentsPlainTextEdit->setMaximumWidth(200);
    auto *closeButton = new QPushButton("Close");
    closeButton->setFixedWidth(50);

    auto *splitter = new QSplitter;
    splitter->addWidget(_treeView);
    splitter->addWidget(_commentsPlainTextEdit);
    splitter->addWidget(closeButton);

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

    connect(_commentsPlainTextEdit, SIGNAL(textChanged()), this, SLOT(onCommentsTextChanged()));

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    loadSettings();
}

void MainWindow::newFileAction() {
    qDebug() << "New file action.";
}

void MainWindow::deleteTaskAction() {
    bool hasSelection = _treeView->selectionModel()->hasSelection();
    if (!hasSelection) {
        QMessageBox::information(this, "Info", "Please select a task to delete.", QMessageBox::Ok);
        return;
    }
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();
    auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
    _model->deleteTask(selectedIndex.row(), selectedIndex);
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

    writeToFile();
}

void MainWindow::onSaveFileAs() {
    QString filter = "Tasklist (*.xml)";
    QString filePath = QFileDialog::getSaveFileName(this, "Save Tasklist As", QDir::homePath(), filter);

    if (filePath.isEmpty()) {
        return;
    }

    _file->setFileName(filePath);

    writeToFile();
}

void MainWindow::writeToFile() {
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

void MainWindow::addTaskAction(bool isSubtask) {
    bool hasSelection = _treeView->selectionModel()->hasSelection();
    if (!hasSelection) {
        QMessageBox::information(this, "Info", "Please select task.", QMessageBox::Ok);
        return;
    }
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();
    auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.first());
    TaskTreeItem *newTaskItem = _model->insertTask(selectedIndex.row(), isSubtask, selectedIndex);

    if (isSubtask) {
        _treeView->expand(selectedIndex);
        selectRow(newTaskItem->row(), selectedIndex);
    } else {
        selectRow(newTaskItem->row(), selectedIndex.parent());
    }
}

void MainWindow::loadModelFromByFilePath(const QString &filePath) {
    if (filePath.isEmpty()) {
        return;
    }

    _statusBar->showMessage(filePath);

    _file->setFileName(filePath);

    if (!_file->exists()) {
        QMessageBox::warning(this, "Error file", "File: '" + filePath + "' not exists.", QMessageBox::Ok);
        _file->close();
        return;
    }

    if (!_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error file", "Failed to open file: '" + filePath + "'.", QMessageBox::Ok);
        _file->close();
        return;
    }

    _model->setModelData(_file);
    _treeView->expandAll();

    if (!_treeView->selectionModel()->hasSelection()) {
        selectRow(0, _model->indexFromItem(_model->getRootItem()));
    }

    _file->close();
}

QMenuBar *MainWindow::createMenuBar() {
    auto *menuBar = new QMenuBar;

    auto *fileMenu = new QMenu("&File");
    fileMenu->addAction(QPixmap(":images/new.png"), "&New Tasklist...", this, SLOT(newFileAction()));
    fileMenu->addSeparator();
    fileMenu->addAction(QPixmap(":images/open.png"), "&Open Tasklist...", this, SLOT(onOpenFileClicked()),
                        tr("Ctrl+O"));
    fileMenu->addAction(QPixmap(":images/save.png"), "&Save Tasklist", this, SLOT(saveAction()), tr("Ctrl+S"));

    fileMenu->addAction("Save Tasklist &As...", this, SLOT(onSaveFileAs()));
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, SLOT(close()), tr("Alt+F4"));

    auto *newTaskMenu = new QMenu("&New Task");
    newTaskMenu->addAction(QPixmap(":images/add.png"), "New Task Below Selected Task", this,
                           [this] { addTaskAction(false); }, tr("Ctrl+N"));
    newTaskMenu->addSeparator();
    newTaskMenu->addAction(QPixmap(":images/add-sub.png"), "New Subtask at Bottom of Selected Task", this,
                           [this] { addTaskAction(true); }, tr("Ctrl+Shift+N"));

    auto *editMenu = new QMenu("&Edit");
    editMenu->addAction(QPixmap(":images/delete.png"), "&Delete Selected Task", this, SLOT(deleteTaskAction()),
                        tr("Delete"));

    auto *toolsMenu = new QMenu("&Tools");
    toolsMenu->addAction(QPixmap(":images/settings.png"), "Preferences...");

    auto *helpMenu = new QMenu("&Help");
    helpMenu->addAction("&About vdh");

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(newTaskMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(toolsMenu);
    menuBar->addMenu(helpMenu);

    return menuBar;
}

QToolBar *MainWindow::createToolBar() {
    auto *toolBar = new QToolBar();
    toolBar->addAction(QPixmap(":images/open.png"), "Open Tasklist (Ctrl+O)", this, SLOT(onOpenFileClicked()));
    toolBar->addAction(QPixmap(":images/save.png"), "Save Tasklist", this, SLOT(saveAction()));
    toolBar->addSeparator();
    toolBar->addAction(QPixmap(":images/add.png"), "New Task", this, [this] { addTaskAction(false); });
    toolBar->addAction(QPixmap(":images/add-sub.png"), "New Subtask", this, [this] { addTaskAction(true); });
    toolBar->addSeparator();
    toolBar->addAction(QPixmap(":images/new.png"), "New Tasklist", this, SLOT(newFileAction()));
    toolBar->addAction(QPixmap(":images/delete.png"), "Delete Task (Delete)", this, SLOT(deleteTaskAction()));
    toolBar->addAction(QPixmap(":images/settings.png"), "Preference", this, SLOT(preferenceAction()));
    addToolBar(toolBar);
    return toolBar;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void MainWindow::writeElement(QXmlStreamWriter &writer, TaskTreeItem *root) {
    for (int i = 0; i < root->childCount(); ++i) {
        TaskTreeItem *child = root->child(i);
        writer.writeStartElement(TASK_ALIAS);
        writer.writeAttribute(ID_ALIAS, QString::number(child->getId()));
        writer.writeAttribute(TITLE_ALIAS, child->getTitle());
        writer.writeAttribute(PARENT_ID_ALIAS, QString::number(child->getParentId()));
        writer.writeAttribute(PRIORITY_ALIAS, QString::number(child->getPriority()));
        writer.writeAttribute(STAR_ALIAS, QString::number(child->getStar()));
        if (child->getDone()) {
            writer.writeAttribute(DONE_ALIAS, QString::number(child->getDone()));
        }
        if (!child->getComments().isEmpty()) {
            writer.writeTextElement(COMMENTS_ALIAS, child->getComments());
        }

        if (child->childCount()) {
            writeElement(writer, child);
        }

        writer.writeEndElement();
    }
}

#pragma clang diagnostic pop

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (selected.isEmpty()) {
        qDebug() << "Selected is null.";
        return;
    }
//    qDebug() << "Selection change:" << selected << "deselected:" << deselected;
    auto &range = const_cast<QItemSelectionRange &>(selected.first());
    const QModelIndexList &list = range.indexes();
    auto &index = const_cast<QModelIndex &>(list.first());
    auto *itemForAttach = static_cast<TaskTreeItem *>(index.internalPointer());
    _commentsPlainTextEdit->setPlainText(itemForAttach->getComments());
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
//    qDebug() << "Insert pTreeItem:";
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

void MainWindow::selectRow(const int row, const QModelIndex &index) {
    QItemSelectionModel *selectionModel = _treeView->selectionModel();
    selectionModel->select(_model->index(row, 0, index),
                           QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void MainWindow::onCommentsTextChanged() {
    const QModelIndexList &selectedIndexesList = _treeView->selectionModel()->selectedIndexes();
    auto &selectedIndex = const_cast<QModelIndex &>(selectedIndexesList.at(
            ColumnsData::getIndexByAlias(COMMENTS_ALIAS)));
    _model->setData(selectedIndex, QVariant(_commentsPlainTextEdit->toPlainText()), Qt::EditRole);
}
