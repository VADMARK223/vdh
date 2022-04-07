//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_TITLEDATA_H
#define VDH_TITLEDATA_H


class TitleData {
public:
    explicit TitleData(QString title = "Task");

    QString getTitle();

    void setTitle(QString value);

    QString toString();

private:
    QString _title;
};


#endif //VDH_TITLEDATA_H
