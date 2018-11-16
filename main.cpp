#include "globals.h"
#include "ztextfilehelper.h"
#include <QCoreApplication>
#include <QTextCodec>
#include <zlog.h>
#include "replaceliteral.h"
#include<cstdio>
#ifdef Q_OS_WIN
#include<windows.h>
#endif

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

        //qDebug() << "Hello World";
    //SetConsoleOutputCP(CP_UTF8);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP_UTF8"));
    //SetConsoleCP(1252);

    ///TODO setlocale
    /// meg kell tudni a rendszer lokalizációt
    /// kell egy text codec a zlogban
    /// mindent ami a konzolra megy, azzal a codeccel kell kirakni
    ///
    /// QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    ///QTextCodec::setCodecForCStrings(codec);
    ///qDebug() << "ÑABCgÓ";
    /// https://www.google.hu/search?q=qdebug+locale&oq=qdebug+locale&aqs=chrome..69i57.4423j0j4&sourceid=chrome&ie=UTF-8
    setlocale(LC_ALL, "Hungarian");
    //SetConsoleOutputCP(1252);

    //zInfo(QStringLiteral("debug teszt"));
    //auto a = zLocInfo(static_cast<const char*>(__PRETTY_FUNCTION__),__FILE__,__LINE__);
    //zTrace();
    int e = ReplaceLiteral::replace();
    return e;//a.exec();
}





