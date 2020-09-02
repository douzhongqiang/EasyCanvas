#ifndef SCHEMEDATAINFO
#define SCHEMEDATAINFO

#include <QDateTime>
#include <QString>
#include <QImage>
#include <QCoreApplication>

class SchemeDataInfo
{
public:
    struct SchemeInfo
    {
        QString schemeName;
        QDateTime createTime;
        QDateTime editTime;

        QImage image;

        bool operator == (const SchemeInfo& info) const
        {
            return schemeName == info.schemeName;
        }

        bool operator < (const SchemeInfo& info) const
        {
            return editTime < info.editTime;
        }
    };
};

//class SchemeDataInfoSort
//{
//public:
//    bool operator () (const SchemeDataInfo::SchemeInfo& a, const SchemeDataInfo::SchemeInfo& b) const
//    {
//        if ()
//    }
//};

inline uint qHash(const SchemeDataInfo::SchemeInfo& schemeInfo, uint seed = 0)
{
    return qHash(schemeInfo.schemeName, seed);
}

#endif
