#ifndef SCHEMEDATAINFO
#define SCHEMEDATAINFO

#include <QDateTime>
#include <QString>

class SchemeDataInfo
{
public:
    struct SchemeInfo
    {
        QString schemeName;
        QDateTime createTime;
        QDateTime editTime;
    };


};


#endif
