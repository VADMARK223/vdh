//
// Created by v.markitanov on 22.03.2022.
//

#include "autoHideMsgBox.h"
#include <iostream>

using namespace std;

void AutoHideMsgBox::setTimeout(int value) {
    timeout = value;
}

void AutoHideMsgBox::showEvent(QShowEvent *event) {
    cout << "Show event timeout=" << timeout << endl;
    currentTime = 0;
    if (timeout / 1000 != -1) {
        startTimer(1000);
    }
    QMessageBox::showEvent(event);
}

void AutoHideMsgBox::timerEvent(QTimerEvent *event) {
//    QObject::timerEvent(event);
    cout << "AutoHideMsgBox timer event." << endl;
    currentTime++;
    if (currentTime >= timeout / 1000) {
        done(0);
    }
}

