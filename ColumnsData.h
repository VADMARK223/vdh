//
// Created by v.markitanov on 05.04.2022.
//

#ifndef VDH_COLUMNSDATA_H
#define VDH_COLUMNSDATA_H

#include <QString>
#include <QVector>

struct COLUMN {
    int index;
    QString name;
    int width;
    bool hide = false;
};

class ColumnsData {
public:
    static QVector<COLUMN> getColumns();
private:
    static const QVector<COLUMN> _columns;
};


#endif //VDH_COLUMNSDATA_H
