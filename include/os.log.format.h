#pragma once

#include <ctime>
#include <string>
#include <string>
#include <sstream>
#include <vector>

namespace imajuscule
{
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
    
    void FormatDate(tm*time, std::string&oDate);
    bool iequals(const std::string& a, const std::string& b);
}