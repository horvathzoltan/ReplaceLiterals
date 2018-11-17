#include "zlog.h"
#include "zstringhelper.h"
#include "ztextfilehelper.h"

#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include "zfilenamehelper.h"

QString zTextFileHelper::load(const QString& filename) {
    QFileInfo fi(filename);
    if(!fi.isAbsolute())
    {
        zInfo(QStringLiteral("path is not absolute: %1").arg(filename));
        return zStringHelper::Empty;
    }

    if(!fi.exists())
    {
        zInfo(QStringLiteral("file not exist: %1").arg(filename));
        return zStringHelper::Empty;
    }

    QFile f(filename);
    QString e;

    // TODO ha relatív a filename, akkor abszolúttá kell tenni
    // egyébként megnyitható azaz

    if (f.open(QFile::ReadOnly | QFile::Text))  {
        zInfo(QStringLiteral("loaded: %1").arg(filename));
        QTextStream st(&f);
        st.setCodec("UTF-8");
        e = st.readAll();
    }
    else{
        zInfo(QStringLiteral("cannot open file: %1 ERROR").arg(filename));
        e= zStringHelper::Empty;
    }
    return e;
}

QStringList zTextFileHelper::loadLines(const QString& filename) {
    QFileInfo fi(filename);
    if(!fi.isAbsolute())
    {
        zInfo(QStringLiteral("pat is not absolute: %1").arg(filename));
        return zStringHelper::EmptyList;
    }

    if(!fi.exists())
    {
        zInfo(QStringLiteral("file not exist: %1").arg(filename));
        return zStringHelper::EmptyList;
    }

    QFile f(filename);
    QStringList e;

    // TODO ha relatív a filename, akkor abszolúttá kell tenni
    // egyébként megnyitható azaz

    if (f.open(QFile::ReadOnly | QFile::Text))  {
        zInfo(QStringLiteral("loaded: %1").arg(filename));
        QTextStream st(&f);
        st.setCodec("UTF-8");

        while (!st.atEnd())
        {
            e << st.readLine();
        }
        f.close();

    }
    else{
        zInfo(QStringLiteral("cannot open file: %1 ERROR").arg(filename));
        e= zStringHelper::EmptyList;
    }
    return e;
}


void zTextFileHelper::save(const QString& txt, const QString& fn, bool isAppend) {

//    QFile logfile(lfn);
//    logfile.open(QIODevice::Append | QIODevice::Text);
//    QTextStream out(&logfile);
//    out << lftxt << endl;

    QFile f(fn);

    auto om = QIODevice::WriteOnly | QIODevice::Text; // openmode
    if(isAppend) om |= QIODevice::Append;

    if (!f.open(om)){
        zError(QStringLiteral("cannot open file to write or append: ")+fn);
        //zLog::dialogError("nem menthető: "+fn);
        return;
        }


    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QTextStream out(&f);
    //out.setCodec(QTextCodec::codecForName("UTF-8"));
    //out.setGenerateByteOrderMark(true);
    out << txt;//.toUtf8();
    f.close();
}

bool zTextFileHelper::backup(const QString& filename)
{
    QFile infile(filename);
    QString outfilename = zFileNameHelper::appendToBaseName(filename, QStringLiteral("old"));

    return QFile::copy(filename, outfilename);
}

