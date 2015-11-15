#pragma once

#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

#include <stdarg.h>
#include "os.log.stack.h"

namespace imajuscule
{
typedef enum logLevel
{
    INFO,
    WARN,
    ERR
}logLevel;

void LG(logLevel, /*const char* sModule,*/ const char * format, ...);

#define ERR_LOG(x,type) do{ \
LG(ERR, "%s (%s) failed\n   |in %s\n   |(%s,%d)", #type, #x, __func__, __FILE__, __LINE__ ); \
logStack(); \
} while(0)

// "soft" Assert
#define C(x) if(!(x)) {ERR_LOG(x,Check);} else do{}while(0)

/* 
"A" is used to replace "assert(x)" with "A(x)"

with the advantage that :
- a comprehensive error message is logged, and the stack is available in debugger (not always the case with assert)
- in release, code execution is not broken but an error message is logged
*/

#ifndef NDEBUG
#define ASSERT_THROW do{throw;}while(0)
#else
#define ASSERT_THROW do{}while(0)
#endif

#define ASSERT_ERR_LOG(x) ERR_LOG(x,Assertion)
#define ASSERT_ERR(x) do{ASSERT_ERR_LOG(x); ASSERT_THROW;}while(0)

#ifdef _MSC_VER
#define likely(x)  x
#define unlikely(x) x
#else
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif

#ifndef NDEBUG
#define if_A(x) if(unlikely(!(x))) ASSERT_ERR(x); else
#else
#define if_A(x)
#endif

#ifndef NDEBUG
#define A(x) if_A(x) do {} while ( 0 )
#else
#define A(x) do {} while ( 0 )
#endif

/* 
"if_A" is used to replace 

//  if(x)
//  {
//      // user code
//  }
//  else
//  {
//      assert(x);
//      LG(ERR,"x is NULL");
//  }

with

//  if_A(x)
//  {
//      // user code
//  }
*/
    
    template <class T>
    void logCoords(const char * message, const T & coords)
    {
        LG(INFO, "%s %f %f %f", message, coords[0], coords[1], coords[2]);
    }
}