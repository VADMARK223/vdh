//
// Created by v.markitanov on 29.03.2022.
//

#ifndef VDH_MAINWINDOW_H
#define VDH_MAINWINDOW_H

#include "TaskTreeModel.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTreeView>

const bool AUTO_LOAD_MODEL = true;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:

    void newFileAction();

    void onOpenFileClicked();

    void onSaveFileAs();

    void loadModelFromByFilePath(const QString &qString);

    void addTaskAction(bool isSubtask);

    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previou);

    void preferenceAction();

    void saveAction();

private:
    QTreeView *_treeView = new QTreeView();
    TaskTreeModel *_model = new TaskTreeModel();

    QMenuBar *createMenuBar();

    QToolBar *createToolBar();

//    void writeElement(TaskTreeItem *pItem);
    void writeElement(QXmlStreamWriter &writer, TaskTreeItem *root);
};

#endif //VDH_MAINWINDOW_H
