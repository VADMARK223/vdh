//
// Created by v.markitanov on 29.03.2022.
//

#ifndef VDH_MAINWINDOW_H
#define VDH_MAINWINDOW_H

#include "../taskTreeModel/TaskTreeModel.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTreeView>

const bool AUTO_LOAD_MODEL = false;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QTreeView * _treeView = new QTreeView();

public slots:

    void onOpenFileClicked();

    void loadModelFromByFilePath(const QString& qString);

    void setModel(TaskTreeModel *pModel);
};

#endif //VDH_MAINWINDOW_H
