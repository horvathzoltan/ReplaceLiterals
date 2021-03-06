#include "globals.h"
#include "zlog.h"
#include "zstringhelper.h"
#include <QTextCodec>
#include <csignal>
#ifdef Q_OS_LINUX
#include <execinfo.h>
#include <cxxabi.h>
#endif

//QTextBrowser* zLog::widget2;
//QTabWidget* zLog::tabwidget2;
//int zLog::tabindex2;

bool zLog::isBreakOnError = false;
zLogGUIfn zLog::GUILogger = nullptr;
void* zLog::ui = nullptr;

void zLog::init(zLogGUIfn ez, bool isBreak, void* uiptr)
{
    GUILogger = ez;
    isBreakOnError = isBreak;
    ui=uiptr;
}

//void zLog::init(QTextBrowser* b, QTabWidget* tw, int tabindex, bool isBreak){
//    widget2=b;
//    tabwidget2 = tw;
//    tabindex2 = tabindex;
//    isBreakOnError = isBreak;
//}

//void zLog::log(const QString& m){
//    #ifdef QT_DEBUG

//    if(m.endsWith("OK")){
//        log(m.left(m.length()-2), OK);
//    }
//    else if(m.endsWith("ERROR")){
//        log(m.left(m.length()-5), ERROR);
//    }
//    else if(m.endsWith("TRACE")){
//        log(m.left(m.length()-5), TRACE);
//    }
//    else{
//        log(m, -1);
//    }
//    #endif
//}

//void zLog::log(const QString& m, int errlevel){
//   // #ifdef QT_DEBUG

//    auto c = widget->textColor();

//    switch(errlevel){

//    case ERROR:
//        widget->setTextColor(QColor(Qt::darkRed));
//        tabwidget->setCurrentIndex(tabindex);
//        break;
//    case WARNING:
//        widget->setTextColor(QColor(Qt::darkYellow));
//        tabwidget->setCurrentIndex(tabindex);
//        break;
//    case TRACE:
//        widget->setTextColor(QColor(Qt::darkGray));
//        break;
//    case DEBUG:
//        widget->setTextColor(QColor(Qt::darkRed));
//        break;
//    case INFO:
//        widget->setTextColor(QColor("steelblue"));
//        break;
//    case OK:
//        widget->setTextColor(QColor(Qt::darkGreen));
//        break;
//    default:
//        widget->setTextColor(QColor(Qt::black));
//        break;
//    }

//    widget->append(m);
//    widget->setTextColor(c);
//    //#endif
//}

//void zLog::log(const QList<QString>& ml){
//    zforeach(m, ml){
//        this->log(*m);
//        }
//}

/*log*/

QString zLog::LevelToString(int i)
{
    switch(i)
    {
        case ERROR:
            return QStringLiteral("Error");
            break;
        case WARNING:
            return QStringLiteral("Warning");
            break;
        case TRACE:
            return QStringLiteral("Trace");
            break;
        case DEBUG:
            return QStringLiteral("Debug");
            break;
        case INFO:
            return QStringLiteral("Info");
            break;
        default:
            return zStringHelper::Empty;
            break;
    }

}







#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
QString zLog::logToString(int errlevel, const QString &msg, const QString &loci, const QString &st)
{
    auto level = LevelToString(errlevel);
    QString msg3;

    switch(errlevel)
    {
    case ERROR:
        msg3= level+": "+msg+"\n"+loci;
        break;
    case WARNING:
        msg3= level+": "+msg+"\n"+loci;
        break;
    case TRACE:
        msg3= level+": "+loci;
        break;
    case DEBUG:
        msg3= level+": "+msg+"\n"+loci;
        break;
    case INFO:
        msg3= msg;
        break;
    default:
        break;
    }

    return msg3;
}
#pragma GCC diagnostic pop


#ifdef Q_OS_LINUX
QString zLog::zStackTrace()
{
    QStringList e;

    unsigned int max_frames = 64;

    e << QStringLiteral("stack trace:");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(static_cast<void**>(addrlist), sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0)
    {
        e << QStringLiteral("<empty, possibly corrupt>");
        return e.join('\n');
    }

    // resolve addresses into strings containing "filename(function+address)", this array must be free()-ed
    auto symbollist = backtrace_symbols(static_cast<void**>(addrlist), addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    auto funcname = static_cast<char*>(malloc(funcnamesize));

    // iterate over the returned symbol lines. skip the first, it is the address of this function.
    for (int i = 1; i < addrlen; i++)
    {
    char *begin_name = nullptr, *begin_offset = nullptr, *end_offset = nullptr;

    for (char *p = symbollist[i]; *p; ++p)
    {
        if (*p == '(')
        {
            begin_name = p;
        }
        else if (*p == '+')
        {
            begin_offset = p;
        }
        else if (*p == ')' && begin_offset)
        {
            end_offset = p;
            break;
        }
    }

    if (begin_name && begin_offset && end_offset
        && begin_name < begin_offset)
    {
        *begin_name++ = '\0';
        *begin_offset++ = '\0';
        *end_offset = '\0';

        int status;
        char* ret = abi::__cxa_demangle(begin_name,funcname, &funcnamesize, &status);
        if (status == 0)
        {
            funcname = ret; // use possibly realloc()-ed string
            //e << QStringLiteral("%1: %2 + %3").arg(symbollist[i],funcname,begin_offset);
            e << QStringLiteral("%1").arg(funcname);
        }
        else
        {
            // demangling failed. Output function name as a C function with
            // no arguments.
            //e << QStringLiteral("%1: %2 + %3").arg(symbollist[i],begin_name,begin_offset);
            e << QStringLiteral("%1").arg(begin_name);
        }
    }
    else
    {
        // couldn't parse the line? print the whole line.
        e << QString(symbollist[i]);
    }
    }

    free(funcname);
    free(symbollist);

    auto a =  e.join('\n');
    return a;
}

#endif

#ifdef Q_OS_WIN
QString zLog::zStackTrace(){
    QStringList e;

   // unsigned int max_frames = 64;

    e << QStringLiteral("stack trace:");

    // WIN implementációt adni
    e << QStringLiteral("?");

    auto a =  e.join('\n');

    return a;
}
#endif
//QString zLog::zStackTrace()
//{
//    QStringList e;
//    void *array[10];
//     size_t size;
//     char **strings;
//     size_t i;

//     size = backtrace (array, 10);
//     strings = backtrace_symbols (array, size);

//     e << QStringLiteral("Obtained %1 stack frames./n").arg(size);

//     for (i = 0; i < size; i++)
//       e<< QStringLiteral("%1\n").arg(strings[i]);

//     free (strings);
//    auto a = e.join("\n");
//    return QString(a);
//}

void zLog::error2(const QString& msg, const zLocInfo& locinfo){
    auto li = locinfo.toString();
    auto st = zLog::zStackTrace();
    //logtogui -> to widget, logtostring
    if(GUILogger!=nullptr)
    {
        GUILogger(ERROR, msg, li, st, ui);
    }
    auto msg2 = logToString(ERROR, msg, li, st);
    // logtosql, csv...
    // logto messagelogger <- logtostring
    // msg2 = logToString(ERROR, msg, li, st);
#ifdef QT_DEBUG
    //qCritical().noquote() << msg2;
    QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, static_cast<const char*>(__PRETTY_FUNCTION__)).critical().noquote()<<msg2;
#ifdef Q_OS_LINUX
    if(isBreakOnError) std::raise(SIGTRAP);
#endif
#endif
    }

void zLog::warning2(const QString& msg, const zLocInfo& locinfo){
    auto li = locinfo.toString();
    if(GUILogger!=nullptr)
    {
        GUILogger(WARNING, msg, li, nullptr, ui);
    }
    auto msg2 = logToString(WARNING, msg, li, nullptr);
#ifdef QT_DEBUG
    //qWarning().noquote() << msg2;
    QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, static_cast<const char*>(__PRETTY_FUNCTION__)).warning().noquote()<<msg2;
#endif
    }

void zLog::info2(const QString& msg)
{
    if(GUILogger!=nullptr)
    {
        GUILogger(INFO, msg, nullptr, nullptr, ui);
    }
    auto msg2 = logToString(INFO, msg, nullptr, nullptr);
#ifdef QT_DEBUG
    //auto a = reinterpret_cast<const char*>(__PRETTY_FUNCTION__); /* NOLINT */
    //auto a = static_cast<const char*>(__PRETTY_FUNCTION__);
    QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, static_cast<const char*>(__PRETTY_FUNCTION__)).info().noquote()<<msg2;
    //qInfo().noquote();// << "a";//msg2;
#else
        QTextStream out(stdout);
        out << msg2<<endl;
#endif
}

void zLog::info2(const QStringList& msgl)
{
    zforeach(msg,msgl)
    {
        if(GUILogger!=nullptr)
        {
            GUILogger(INFO, *msg, nullptr, nullptr, ui);
        }
        auto msg2 = logToString(INFO, *msg, nullptr, nullptr);
#ifdef QT_DEBUG
        QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, static_cast<const char*>(__PRETTY_FUNCTION__)).info().noquote()<<msg2;
        //qInfo();//.noquote() << msg2;
#else
        QTextStream out(stdout);
        out << msg2<<   endl;
#endif
    }
}

void zLog::debug2(const zLocInfo& locinfo){
    auto li = locinfo.toString();
    auto st = zLog::zStackTrace();
    if(GUILogger!=nullptr)
    {
        GUILogger(DEBUG, nullptr, li, st, ui);
    }
    auto msg2 = logToString(DEBUG, nullptr, li, st);
#ifdef QT_DEBUG
    //qDebug().noquote() << msg2;
    QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, static_cast<const char*>(__PRETTY_FUNCTION__)).debug().noquote()<<msg2;
#ifdef Q_OS_LINUX
    if(isBreakOnError) std::raise(SIGTRAP);
#endif
#endif
    }

void zLog::trace2(const zLocInfo& locinfo){
    auto li = locinfo.toString();
    if(GUILogger!=nullptr)
    {
        GUILogger(TRACE, nullptr, li, nullptr, ui);
    }
    auto msg2 = logToString(TRACE, nullptr, li, nullptr);
#ifdef QT_DEBUG
   //qDebug().noquote() << msg2;
    QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, static_cast<const char*>(__PRETTY_FUNCTION__)).debug().noquote()<<msg2;
#endif
    }

/*
 * __PRETTY_FUNCTION__
 *
https://gcc.gnu.org/onlinedocs/cpp/Predefined-Macros.html

*/
