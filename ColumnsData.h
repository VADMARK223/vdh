//
// Created by v.markitanov on 05.04.2022.
//

#ifndef VDH_COLUMNSDATA_H
#define VDH_COLUMNSDATA_H

#include <QString>
#include <QVector>

#define ID_ALIAS "ID_ALIAS"
#define PARENT_ID_ALIAS "PARENT_ID_ALIAS"
#define DEPTH_ALIAS "DEPTH_ALIAS"
#define COMMENTS_ALIAS "COMMENTS_ALIAS"

struct COLUMN {
    int index;
    QString alias;
    QString name;
    int width;
    bool hide = false;
};

class ColumnsData {
public:
    static QVector<COLUMN> getColumns();
    static int getIndexByAlias(QString alias); // TODO: Need map
private:
    static const QVector<COLUMN> _columns;
};


#endif //VDH_COLUMNSDATA_H
