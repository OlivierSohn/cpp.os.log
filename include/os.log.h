#pragma once

#include <stdarg.h>

typedef enum logLevel
{
    INFO,
    WARN,
    ERR
}logLevel;

void LG(logLevel, /*const char* sModule,*/ const char * format, ...);
