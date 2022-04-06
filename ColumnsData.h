//
// Created by v.markitanov on 05.04.2022.
//

#ifndef VDH_COLUMNSDATA_H
#define VDH_COLUMNSDATA_H

#include <QString>
#include <QVector>

#define TITLE_ALIAS "TITLE"
#define ID_ALIAS "ID"
#define PARENT_ID_ALIAS "P"
#define DEPTH_ALIAS "DEPTH_ALIAS"
#define COMMENTS_ALIAS "COMMENTS"
#define TASK_ALIAS "TASK"

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

    static COLUMN createColumn(int index, const char *alias, const char *name, int width, bool hide);
};


#endif //VDH_COLUMNSDATA_H
