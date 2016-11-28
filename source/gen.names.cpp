#include "gen.names.h"

using namespace std;

namespace imajuscule
{
    enum asciiAlphaLower { Min = ascii<'a'>::value, Max = ascii<'z'>::value };
    
    int shortIds::count = 0;
    char shortIds::c = asciiAlphaLower::Min;
    
    string shortIds::generate() {
        string ret = toString();
        increment();
        return ret;
    }
    string shortIds::toString() {
        string ret(1,c);
        if(count) {
            ret += to_string(count);
        }
        return ret;
    }
    void shortIds::increment() {
        if(c == asciiAlphaLower::Max) {
            c = asciiAlphaLower::Min;
            count ++;
        } else {
            c++;
        }
    }
}
