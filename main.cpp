#include "globals.h"
#include "ztextfilehelper.h"
#include <QCoreApplication>
#include <QTextCodec>
#include <zlog.h>
#include "replaceliteral.h"
#include <QCommandLineParser>
#include<cstdio>
#ifdef Q_OS_WIN
#include<windows.h>
#endif

int main(int argc, char *argv[])
{
    zInfo("started");

    QCoreApplication app(argc, argv);


    QCommandLineParser parser;
    QCoreApplication::setApplicationName(QStringLiteral("ReplaceLiteral"));
    parser.setApplicationDescription(QStringLiteral("Replace Literals by localized message file contains translated messages and its keys in csv format."));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption m_opt(
                QStringList {"m" , "message"},
                QStringLiteral("translated messages"),
                QStringLiteral("messages")
                );
    parser.addOption(m_opt);

    QCommandLineOption s_opt(
                QStringList {"s", "source"},
                QStringLiteral("source file"),
                QStringLiteral("source")
                );

    parser.addOption(s_opt);

    QCommandLineOption b_opt(
                QStringLiteral("backup"),
                QStringLiteral("backup file"));

    parser.addOption(b_opt);

    parser.process(app);

    QString lFileName = parser.value(m_opt);
    QString sFileName = parser.value(s_opt);
    bool isBackup= parser.isSet(b_opt);

    if(isBackup)
    {
        if(zTextFileHelper::backup(sFileName))
        {
            zInfo(QStringLiteral("backup file copied"));
        }
    }

    int e = ReplaceLiteral::replace(lFileName, sFileName);



    system("pause");

    return (e>0)?0:1;//app.exec();
}





