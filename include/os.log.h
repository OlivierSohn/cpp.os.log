#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

namespace imajuscule
{
typedef enum logLevel
{
    SCRIPT = 1,
    INFO,
    WARN,
    ERR = 0
}logLevel;

void LG(logLevel, /*const char* sModule,*/ const char * format, ...);

#define ERR_LOG(x,type) do{ \
LG(ERR, "%s : ' %s '\n   |in %s\n   |(%s,%d)", #type, #x, __func__, __FILE__, __LINE__ ); \
logStack(); \
} while(0)

// "soft" Assert
#define C(x) if(!(x)) {ERR_LOG(x,check);} else do{}while(0)

/* 
"A" is used to replace "assert(x)" with "A(x)"

with the advantage that :
- a comprehensive error message is logged, and the stack is available in debugger (not always the case with assert)
- in release, code execution is not broken but an error message is logged
*/

#ifndef NDEBUG
#define ASSERT__THROW do{throw;}while(0)
#else
#define ASSERT__THROW do{}while(0)
#endif

#define ASSERT_ERR_LOG(x) ERR_LOG(x,assert)
#define ASSERT_ERR(x) do{ASSERT_ERR_LOG(x); ASSERT__THROW;}while(0)

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

    struct Logger {
        template <class... Args>
        static void err(Args&&... args) {
            LG(ERR, string_format(std::forward<Args>(args)...).c_str());
        }
    };

    template <class T>
    void logCoords(const char * message, const T & coords) {
        LG(INFO, "%s % .3f % .3f % .3f", message, coords[0], coords[1], coords[2]);
    }
    
    template <class T>
    void logQuaternion(const char * message, const T & coords) {
        LG(INFO, "%s %.3f %.3f %.3f %.3f", message, coords[0], coords[1], coords[2], coords[3]);
    }
    
}
