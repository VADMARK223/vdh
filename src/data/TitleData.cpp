//
// Created by v.markitanov on 07.04.2022.
//

#include <QString>
#include <utility>
#include "TitleData.h"

TitleData::TitleData(QString title, bool done) : _title(std::move(title)), _done(done) {

}

QString TitleData::getTitle() {
    return _title;
}

QString TitleData::toString() {
    return "TitleData(" +
           this->getTitle() + "," +
           QString::number(this->getDone()) +
           ")";
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
