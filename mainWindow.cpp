//
// Created by v.markitanov on 22.03.2022.
//
#include "mainWindow.h"
#include "autoHideMsgBox.h"
#include <iostream>

#include <QHBoxLayout>

using namespace std;

MainWindow::MainWindow() {
    connect(pStartBtn, SIGNAL(clicked(bool)), this, SLOT(startBtnClickedHandler()));

    auto *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(pStartBtn);
    setLayout(pHLayout);

    pFinishBtn->setEnabled(false);
    connect(pFinishBtn, SIGNAL(clicked(bool)), this, SLOT(finishBtnClickedHandler()));
    pHLayout->addWidget(pFinishBtn);

    auto *pCloseBtn = new QPushButton("E&xit");
    connect(pCloseBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    pHLayout->addWidget(pCloseBtn);

    connect(timer, &QTimer::timeout, this, &MainWindow::timerHandler);
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


