//
// Created by v.markitanov on 07.04.2022.
//

#include <QString>
#include <utility>
#include "TitleData.h"

TitleData::TitleData(QString title, bool done, QString comments) :
        _title(std::move(title)),
        _done(done),
        _comments(std::move(comments)) {
}

QString TitleData::getTitle() {
    return _title;
}

void TitleData::setTitle(QString value) {
    _title = std::move(value);
}

bool TitleData::getDone() {
    return _done;
}

void TitleData::setDone(bool value) {
    _done = value;
}

QString TitleData::getComments() {
    return _comments;
}

void TitleData::setComments(QString value) {
    _comments = std::move(value);
}

QString TitleData::toString() {
    return "TitleData(" +
           this->getTitle() + "," +
           QString::number(this->getDone()) + "," +
           this->getComments() +
           ")";
}
