//
// Created by v.markitanov on 22.03.2022.
//
#include "mainWindow.h"
#include "autoHideMsgBox.h"
#include <iostream>

#include <QHBoxLayout>

using namespace std;

MainWindow::MainWindow() {
    pFinishBtn->setEnabled(false);

    auto *pCloseBtn = new QPushButton("E&xit");

    auto *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(pStartBtn);
    pHLayout->addWidget(pFinishBtn);
    pHLayout->addWidget(pCloseBtn);

    connect(pStartBtn, SIGNAL(clicked(bool)), this, SLOT(startBtnClickedHandler()));
    connect(pFinishBtn, SIGNAL(clicked(bool)), this, SLOT(finishBtnClickedHandler()));
    connect(pCloseBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(timer, &QTimer::timeout, this, &MainWindow::timerHandler);

    setLayout(pHLayout);
}

void MainWindow::startBtnClickedHandler() {
    pStartBtn->setEnabled(false);
    pFinishBtn->setEnabled(true);

    QTimer::singleShot(0, this, &MainWindow::timerHandler);
    timer->start(TIMEOUT);
}

void MainWindow::finishBtnClickedHandler() {
    pStartBtn->setEnabled(true);
    pFinishBtn->setEnabled(false);

    timer->stop();
}

void MainWindow::timerHandler() {
    cout << "Timer handler." << endl;

    AutoHideMsgBox autoHideMsgBox;
    autoHideMsgBox.setText("I am online!");
    autoHideMsgBox.setTimeout(TIMEOUT / 2);
    autoHideMsgBox.exec();
}


