#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

namespace imajuscule {
    typedef enum logLevel
    {
        SCRIPT = 1,
        INFO,
        WARN,
        ERR = 0
    }logLevel;

    void LG(logLevel, /*const char* sModule,*/ const char * format, ...);

    template <class T>
    void logCoords(const char * message, const T & coords) {
        LG(INFO, "%s %.3f %.3f %.3f", message, coords[0], coords[1], coords[2]);
    }

    template<typename T>
    void print_time(std::chrono::time_point<T> time) {
        using namespace std;
        using namespace std::chrono;
        
        time_t curr_time = T::to_time_t(time);
        char sRep[100];
        // if needed use %Y-%m-%d for year / month / date
        strftime(sRep,sizeof(sRep),"%H:%M:%S",localtime(&curr_time));
        
        typename T::duration since_epoch = time.time_since_epoch();
        seconds s = duration_cast<seconds>(since_epoch);
        since_epoch -= s;
        milliseconds milli = duration_cast<milliseconds>(since_epoch);
        
        cout << sRep << ":";
        auto c = milli.count();
        if(c < 100) {
            std::cout << "0";
        }
        if(c < 10) {
            std::cout << "0";
        }
        std::cout << c << "|";
    }
    
    static inline void print_system_time() {
        print_time(std::chrono::system_clock::now());
    }
}
