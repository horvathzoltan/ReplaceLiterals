#ifndef REPLACELITERAL_H
#define REPLACELITERAL_H

#include <QString>

class ReplaceLiteral
{
public:
    ReplaceLiteral();
    static int replace(const QString&, const QString&);
    static QMap<QString,QString> loadmap(QString mapfilename);

    static int doReplace(const QString&, const QMap<QString,QString>&);
    static QString getMessageFileName(const QString&, const QString&);
};

#endif // REPLACELITERAL_H
