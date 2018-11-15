#ifndef GLOBALS_H
#define GLOBALS_H

#include "zlog.h"
#include <QDebug>

#define zforeach(var, container) for(auto (var) = (container).begin(); (var) != (container).end(); ++(var))
#define zforeach_from(var, container, ix) for(auto (var) = (container).begin()+(ix); (var) != (container).end(); ++(var))


#endif // GLOBALS_H
