#include "zfilenamehelper.h"

#include <QFileInfo>
#include <QDir>

zFileNameHelper::zFileNameHelper()
{

}


QString zFileNameHelper::appendToBaseName(const QString& fileName, const QString& a)
{
    QFileInfo fi(fileName);

    QString ns = QFileInfo(fi.dir(), fi.baseName()+"_"+a+"."+fi.completeSuffix()).filePath();

    return ns;
}

