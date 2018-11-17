#ifndef ZFILENAMEHELPER_H
#define ZFILENAMEHELPER_H

#include <QString>



class zFileNameHelper
{
public:
    zFileNameHelper();

    static QString appendToBaseName(const QString& fileName, const QString& a );
};

#endif // ZFILENAMEHELPER_H
