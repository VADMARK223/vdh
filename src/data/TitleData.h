//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_TITLEDATA_H
#define VDH_TITLEDATA_H


class TitleData {
public:
    explicit TitleData(QString title = "Task", bool done = false);

    QString getTitle();

    void setTitle(QString value);

    bool getDone();

    void setDone(bool value);

    QString toString();

private:
    QString _title;

    bool _done = false;
};


#endif //VDH_TITLEDATA_H
