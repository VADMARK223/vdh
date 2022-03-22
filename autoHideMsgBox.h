//
// Created by v.markitanov on 22.03.2022.
//

#ifndef VDH_AUTOHIDEMSGBOX_H
#define VDH_AUTOHIDEMSGBOX_H

#include <QMessageBox>

class AutoHideMsgBox : public QMessageBox {
private:
    int timeout = -1;
    int currentTime = 0;

public:
    void setTimeout(int value);

protected:
    void showEvent(QShowEvent *event) override;

    void timerEvent(QTimerEvent *event) override;
};

#endif //VDH_AUTOHIDEMSGBOX_H
