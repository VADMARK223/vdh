//
// Created by v.markitanov on 22.03.2022.
//

#ifndef VDH_MAINWINDOWOLD_H
#define VDH_MAINWINDOWOLD_H

#include <QDialog>
#include <QPushButton>
#include <QTimer>

class MainWindowOld : public QDialog {
Q_OBJECT

private:
    QPushButton *pStartBtn = new QPushButton("&Start");
    QPushButton *pFinishBtn = new QPushButton("&Finish");
    QTimer *timer = new QTimer(this);

    void timerHandler();

public:
    static const int TIMEOUT = 4000;

    MainWindowOld();

public slots:

    void startBtnClickedHandler();

    void finishBtnClickedHandler();
};

#endif //VDH_MAINWINDOWOLD_H
