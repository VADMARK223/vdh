//
// Created by v.markitanov on 07.04.2022.
//

#include <QString>
#include <utility>
#include "TitleData.h"

TitleData::TitleData(QString title) : _title(std::move(title)) {

}

QString TitleData::getTitle() {
    return _title;
}

QString TitleData::toString() {
    return "TitleData(" +
           this->getTitle() +
           ")";
}

void TitleData::setTitle(QString value) {
    _title = std::move(value);
}

