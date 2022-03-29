//
// Created by v.markitanov on 22.03.2022.
//
#include "mainWindowOld.h"
#include "autoHideMsgBox.h"
#include <iostream>

#include <QHBoxLayout>

using namespace std;

MainWindowOld::MainWindowOld() {
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

    connect(timer, &QTimer::timeout, this, &MainWindowOld::timerHandler);
}

void MainWindowOld::startBtnClickedHandler() {
    pStartBtn->setEnabled(false);
    pFinishBtn->setEnabled(true);

    QTimer::singleShot(0, this, &MainWindowOld::timerHandler);
    timer->start(TIMEOUT);
}

void MainWindowOld::finishBtnClickedHandler() {
    pStartBtn->setEnabled(true);
    pFinishBtn->setEnabled(false);

    timer->stop();
}

void MainWindowOld::timerHandler() {
    cout << "Timer handler." << endl;

    auto *autoHideMsgBox = new AutoHideMsgBox(TIMEOUT / 2, this);
    autoHideMsgBox->show();
}


