//
// Created by v.markitanov on 22.03.2022.
//

#ifndef VDH_MAINWINDOW_H
#define VDH_MAINWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QTimer>

class MainWindow : public QDialog {
Q_OBJECT

private:
    QPushButton *pStartBtn = new QPushButton("&Start");
    QPushButton *pFinishBtn = new QPushButton("&Finish");
    QTimer *timer = new QTimer(this);

    static void timerHandler();

public:
    static const int TIMEOUT = 4000;
    MainWindow();

public slots:

    void startBtnClickedHandler();

    void finishBtnClickedHandler();
};

#endif //VDH_MAINWINDOW_H
