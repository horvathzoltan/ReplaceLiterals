#include "globals.h"
#include "replaceliteral.h"
#include "ztextfilehelper.h"
#include "zfilenamehelper.h"

#include <QDir>
#include <QFileInfo>

ReplaceLiteral::ReplaceLiteral() = default;


//TODO paraméterben kap egy lang fájlt és egy txt
/// ha létezik beolvassa listába
/// ha nem üres és első karakter nem #
/// 1. kulcs
/// 2. érték
/// 3. ha van # visszafele
/// akkor az a fordítás
/// //ezt fordítva kellene,
/// 1. kulcs
/// 2. fordítás
/// ha van # akkor
/// 3. eredeti
/// ezzel csinál két egy mapot - egy erdedetit
/// majd veszi a txt
/// végigmegy a mapon
/// és egyenként ahol a value szerepel, cseréli a keyre
/// majd kiírja a fájlt
///
///
/// impozáns lenne, ha a language fájlban benne lenne a source relatív pojectdir-el
/// így ha a másolatot fordítjuk, mozgatjuk, akkor is tud cserélni
///
/// amikor a mapot generáljuk,
/// feldolgozás során jön -e be path, aminek a vége classname.cs?
/// ha igen, megvan a forrás
/// márpedig, ha LogService hívás van, fog bejönni, az opcionális paraméter miatt
/// amit a fordító kvázi literálisként fordít be
///
/// a cserélő kiolvassa a path-t, hozzáteszi a projectdirt,
/// készít egy másolatot a fájlból - fájlnévben időbélyeggel
/// majd cserél
///
/// tehát meg kell adni a project dirt, a backup dirt, és a nyelvi fájlt

int ReplaceLiteral::replace(const QString& lFileName, const QString& sFileName){

    if(lFileName.isEmpty())
    {
        zInfo(QStringLiteral("no message filename"));
        return 0;
    }

    if(sFileName.isEmpty())
    {
        zInfo(QStringLiteral("no source filename"));
        return 0;
    }

    //QString lFileName = getMessageFileName(lFilePath, sFileName);//Messages_USERSERVICE_hu-HU.csv

    auto map = loadmap(lFileName);
    if(map.isEmpty()) return 1;
    zInfo(QStringLiteral("%1 definitions loaded").arg(map.count()));

    int e = doReplace(sFileName, map);


    return e;
}


QMap<QString,QString> ReplaceLiteral::loadmap(QString mapFileName)
{
    auto maplines = zTextFileHelper::loadLines(mapFileName);

    QMap<QString,QString> map;
    if(maplines.isEmpty())
    {
        zInfo(QStringLiteral("no loaded messages"));
        map;
    }

    /// e.Key + ";" +
    /// ha van fordítás: tt+"#" + e.Value, tehát key;tt#original
    /// ha nincs: e.Value, tehát key;original
    ///
    /// majd az originalt kell cserélni keyre
    zforeach(l, maplines)
    {
        if(!l->isEmpty() && l[0]!='#'){
            int ix = (*l).indexOf(';');
            if(ix>0)
            {
                QString k = l->mid(0, ix).trimmed();
                QString v = l->mid(ix + 1);
                int ix1 = v.lastIndexOf('#');
                if (ix1 > 0)
                {
                    // de mivel nekünk az original kell és az a # után van,
                    v = v.mid(ix1+1);
                }
                if (!k.contains(' '))
                {
                    map.insert(k, v);
                }
            }
        }
    }
    return map;
}

int ReplaceLiteral::doReplace(const QString& sFileName, const QMap<QString,QString>& map)
{
    if(map.isEmpty())
    {
        zInfo(QStringLiteral("empty message map"));
        return 0;
    }
    auto s = zTextFileHelper::load(sFileName);
    if(s.isEmpty())
    {
        zInfo(QStringLiteral("empty source text"));
        return 0;
    }

    int e = 0;
    zforeach(m, map)
    {
        QString ezt = "\""+m.value()+"\"";
        QString erre = "tr(tre."+m.key()+")";
        int i = s.count(ezt);
        s.replace(ezt, erre);
        e += i-s.count(ezt);
    }

    //QFileInfo fi(sFileName);
    //QFileInfo(fi.dir(), fi.baseName()+"_x."+fi.completeSuffix()).filePath();

    //QString ns = zFileNameHelper::appendToBaseName(sFileName, QStringLiteral("x"));
    zTextFileHelper::save(s, sFileName);
    return e;
}

/*
QString lFilePath = QStringLiteral(R"(/home/zoli/mestercipo/mestercipo/Messages)");
QString sFileName = QStringLiteral(R"(/home/zoli/mestercipo/mestercipo/UserService.cs)");
*/
QString ReplaceLiteral::getMessageFileName(const QString& lFilePath, const QString& sFileName)
{
    //Messages_USERSERVICE_hu-HU.csv

    QFileInfo fi(sFileName);

    //auto fn = fi.baseName().toUpper();

    QFileInfo fs(lFilePath);

    QString ns = QFileInfo(fs.filePath(), "Messages_"+fi.baseName().toUpper()+"_hu-HU.csv").filePath();
    zInfo(QStringLiteral("filename: %1").arg(ns));
    return ns;
}
