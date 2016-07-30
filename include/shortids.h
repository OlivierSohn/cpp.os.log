#pragma once

#include <string>

namespace imajuscule {
    class shortIds {
    public:
        static std::string generate();
    private:
        static std::string toString();
        static void increment();
        static int count;
        static char c;
    };
}
