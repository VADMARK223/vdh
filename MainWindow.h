//
// Created by v.markitanov on 29.03.2022.
//

#ifndef VDH_MAINWINDOW_H
#define VDH_MAINWINDOW_H

#include "TaskTreeModel.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTreeView>
#include <QSettings>
#include <QStatusBar>

//const bool AUTO_LOAD_MODEL = true;

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

    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                       const QList<int> &roles = QList<int>());

    void onRowsInserted(const QModelIndex &parent, int first, int last);

    void onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);

    void preferenceAction();

    void saveAction();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QSettings *_settings = new QSettings("settings.ini", QSettings::IniFormat);
    QFile *_file = new QFile(this);
    QTreeView *_treeView = new QTreeView(this);
    TaskTreeModel *_model = new TaskTreeModel(this);
    QStatusBar *_statusBar = new QStatusBar(this);

    void loadSettings();

    void writeSettings();

    QMenuBar *createMenuBar();

    QToolBar *createToolBar();

    void writeElement(QXmlStreamWriter &writer, TaskTreeItem *root);
};

#endif //VDH_MAINWINDOW_H
