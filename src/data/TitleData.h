//
// Created by v.markitanov on 07.04.2022.
//

#ifndef VDH_TITLEDATA_H
#define VDH_TITLEDATA_H


class TitleData {
public:
    explicit TitleData(QString title = "Task", bool done = false, QString comments = "");

    QString getTitle();

    void setTitle(QString value);

    bool getDone();

    void setDone(bool value);

    void setComments(QString value);

    QString getComments();

    QString toString();

private:
    QString _title;

    bool _done = false;

    QString _comments;
};


#endif //VDH_TITLEDATA_H
