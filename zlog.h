#ifndef ZERROR_H
#define ZERROR_H

#include "zlocinfo.h"

#if defined(Q_OS_LINUX)
#define getLocInfo zLocInfo(static_cast<const char*>(__PRETTY_FUNCTION__),__FILE__,__LINE__)
#elif defined(Q_OS_WIN)
#define getLocInfo zLocInfo(__PRETTY_FUNCTION__ ,__FILE__,__LINE__)
#endif


#define zError(msg) zLog::error2((msg), getLocInfo)

#define zWarning(msg) zLog::warning2((msg), getLocInfo);
#define zInfo(msg) zLog::info2((msg));
#define zDebug() zLog::debug2(getLocInfo);
#define zTrace() zLog::trace2(getLocInfo);

typedef void (*zLogGUIfn)(int errlevel, const QString &msg, const QString &loci, const QString &st, void *ui);

class zLog
{
private:
    static zLogGUIfn GUILogger;
    static void dialog(const QString&, int);
    static bool isBreakOnError;
    static void *ui;

    static QString logToString(int, const QString&, const QString&, const QString&);
    static QString zStackTrace();

public:
    enum ErrLevels:int {ERROR, WARNING, TRACE, DEBUG, INFO};

    static QString LevelToString(int loglevel);

    static void init(zLogGUIfn ez, bool isBreak, void* ui);

    /*message*/
    static void dialogMessage(const QString& str);
    static void dialogOk(const QString& str);
   /*warning*/
    static void dialogWarning(const QString& str);
//    /*error*/
    static void dialogError(const QString& str);
//    /*trace*/
    static void dialogTrace(const QString& str);

    static void error2(const QString& msg, const zLocInfo& l);
    static void warning2(const QString& msg, const zLocInfo& l);
    static void info2(const QString& msg);
    static void info2(const QStringList& msg);
    static void debug2(const zLocInfo& l);
    static void trace2(const zLocInfo& l);
};

#endif // ZERROR_H
