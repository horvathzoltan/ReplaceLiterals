#include "globals.h"
#include "replaceliteral.h"
#include "ztextfilehelper.h"

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
int ReplaceLiteral::replace(){
    QString lFileName = QStringLiteral(R"(C:\mestercipo\Messages\Messages_USERSERVICE_hu-HU.csv)");
    QString sFileName = QStringLiteral(R"(C:\mestercipo\UserService.cs)");

    auto map = loadmap(lFileName);
    if(map.isEmpty()) return 1;
    zInfo(QStringLiteral("%1 definitions loaded").arg(map.count()));

    int e = doReplace(sFileName, map);

    zInfo(QStringLiteral("%1 definitions replaced").arg(e));

    return 0;
}


QMap<QString,QString> ReplaceLiteral::loadmap(QString mapFileName)
{
    auto maplines = zTextFileHelper::loadLines(mapFileName);

    QMap<QString,QString> map;
    if(maplines.isEmpty())
    {
        zInfo(QStringLiteral("nem lettek sorok beolvasva"));
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
        zInfo(QStringLiteral("empty map"));
        return 0;
    }
    auto s = zTextFileHelper::load(sFileName);
    if(s.isEmpty())
    {
        zInfo(QStringLiteral("empty source"));
        return 0;
    }
    zforeach(m, map)
    {
        s.replace("\""+m.value()+"\"", "tr(tre."+m.key()+")");
    }

    zTextFileHelper::save(s, sFileName+"x");
}
