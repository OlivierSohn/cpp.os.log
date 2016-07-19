#include "os.log.format.h"
#include "os.log.h"

#include "os.log.content.h"

namespace imajuscule
{
    void logWithLineNumbers(std::string const & str) {
        int number = 0;
        for(auto const & line : split_in_lines(str)) {
            LG(INFO, "%.3d %s", number, line.c_str());
            ++number;
        }
    }
}
