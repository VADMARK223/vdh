//
// Created by v.markitanov on 05.04.2022.
//

#include "ColumnsData.h"
#include <QDebug>

const QVector<COLUMN> ColumnsData::_columns = [] { // NOLINT(cert-err58-cpp)
    QVector<COLUMN> result;

    result << createColumn(static_cast<int>(result.size()), TITLE_ALIAS, "Title", 150, false);
    result << createColumn(static_cast<int>(result.size()), ID_ALIAS, "ID", 30, false);
    result << createColumn(static_cast<int>(result.size()), PARENT_ID_ALIAS, "Parent ID", 60, false);
    result << createColumn(static_cast<int>(result.size()), DEPTH_ALIAS, "Depth", 150, false);
    result << createColumn(static_cast<int>(result.size()), COMMENTS_ALIAS, "Comments", 50, false);

    return result;
}();

QVector<COLUMN> ColumnsData::getColumns() {
    return _columns;
}

int ColumnsData::getIndexByAlias(QString alias) {
    for (const auto &column: _columns) {
        if (column.alias == alias) {
            return column.index;
        }
    }

    return -1;
}

COLUMN ColumnsData::createColumn(int index, const char *alias, const char *name, int width, bool hide) {
    COLUMN column;
    column.index = index;
    column.alias = alias;
    column.name = name;
    column.width = width;
    column.hide = hide;
    return column;
}

