//
// Created by v.markitanov on 29.03.2022.
//

#ifndef VDH_MAINWINDOW_H
#define VDH_MAINWINDOW_H

#include <QMainWindow>
#include "../taskTreeModel/TaskTreeModel.h"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

    static TaskTreeModel * createModel();
};

#endif //VDH_MAINWINDOW_H
