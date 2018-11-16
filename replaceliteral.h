#ifndef REPLACELITERAL_H
#define REPLACELITERAL_H

#include <QString>

class ReplaceLiteral
{
public:
    ReplaceLiteral();
    static int replace();
    static QMap<QString,QString> loadmap(QString mapfilename);

    static int doReplace(const QString&, const QMap<QString,QString>&);
};

#endif // REPLACELITERAL_H
