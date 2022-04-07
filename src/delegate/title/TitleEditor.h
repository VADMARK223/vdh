//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_TITLEEDITOR_H
#define VDH_TITLEEDITOR_H

#include <QWidget>
#include <QtWidgets>

class TitleEditor : public QWidget {
Q_OBJECT
public:
    explicit TitleEditor(QWidget *parent = nullptr);
private:
    QCheckBox *_checkBox;
    QLineEdit *_lineEdit;
};


#endif //VDH_TITLEEDITOR_H
