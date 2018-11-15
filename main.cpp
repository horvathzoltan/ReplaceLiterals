#include <QCoreApplication>
#include <zlog.h>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //qDebug() << "Hello World";

    zInfo(QStringLiteral("debug teszt"));
    //auto a = zLocInfo(static_cast<const char*>(__PRETTY_FUNCTION__),__FILE__,__LINE__);
    //zTrace();

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
    return 1;//a.exec();
}




