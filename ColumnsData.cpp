//
// Created by v.markitanov on 05.04.2022.
//

#include "ColumnsData.h"
#include <QDebug>

const QVector<COLUMN> ColumnsData::_columns = [] { // NOLINT(cert-err58-cpp)
    QVector<COLUMN> result;
    COLUMN column0;
    column0.index = static_cast<int>(result.size());
    column0.name = "ID";
    column0.width = 150;
    result << column0;

    COLUMN column1;
    column1.index = static_cast<int>(result.size());
    column1.name = "Parent ID";
    column1.width = 60;
    result << column1;

    COLUMN column2;
    column2.index = static_cast<int>(result.size());
    column2.name = "Depth";
    column2.width = 50;
    column2.hide = true;
    result << column2;

    COLUMN column3;
    column3.index = static_cast<int>(result.size());
    column3.name = "Comments";
    column3.width = 50;
    result << column3;
    return result;
}();

QVector<COLUMN> ColumnsData::getColumns() {
    return _columns;
}

