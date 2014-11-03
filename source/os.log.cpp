#include "os.log.h"
#include <iostream>

#define LOG_TAG "Grid3D"

// TODO redirect std::out and std::err to android logging for android platform

#ifdef __ANDROID__
#include <android/log.h>
int toAndroid(logLevel i)
{
    switch(i)
    {
    case INFO:
        return ANDROID_LOG_INFO;
    case WARN:
        return ANDROID_LOG_WARN;
    default:
    case ERR:
        return ANDROID_LOG_ERROR;
    }
}
#endif

void LG(logLevel level, /*const char* sModule,*/ const char * txt)
{
#ifdef __ANDROID__
#ifndef NDEBUG
    __android_log_write( toAndroid(level), LOG_TAG, txt);
#endif
#endif
}

const char * levelToChar(logLevel level);

void LG(logLevel level, /*const char* sModule,*/ const char * format, ...)
{
#ifdef __ANDROID__
    va_list args;

    va_start(args, format);
    __android_log_vprint(toAndroid(level), LOG_TAG, format, args);
    va_end(args);
#else
    fprintf(((level==ERR)?stderr : stdout), levelToChar(level));

    va_list args;

    va_start(args, format);
    vfprintf(((level == ERR) ? stderr : stdout), format, args);
    va_end(args);

    fprintf(((level == ERR) ? stderr : stdout), "\n");
#endif
}

const char * levelToChar(logLevel level)
{
    switch (level)
    {
    default:
    case INFO:
        return "INFO|";
    case ERR:
        return "ERR|";
    case WARN:
        return "WARN|";
    }
}
// FOR LATER:
/*
#ifdef ANDROID
#  define LOGI(...) LG(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#  define LOGW(...) LG(WARN, LOG_TAG, __VA_ARGS__)
#  define LOGE(...) LG(ERR, LOG_TAG, __VA_ARGS__)
#else
#  define QUOTEME_(x) #x
#  define QUOTEME(x) QUOTEME_(x)
#  define LOGI(...) printf("I/" LOG_TAG " (" __FILE__ ":" QUOTEME(__LINE__) "): " __VA_ARGS__)
#  define LOGE(...) printf("E/" LOG_TAG "(" ")" __VA_ARGS__)
#endif
*/

