#pragma once

#include <ctime>
#include <string>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>

namespace imajuscule
{
    void split_in_lines(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split_in_lines(const std::string &s, char delim);
    std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters = " ");
    
    void FormatDate(tm*time, std::string&oDate);
    void FormatDateForComparison(std::string & date);
    bool iequals(const std::string& a, const std::string& b);
    
    
    template<char ... CharacterList>
    inline bool check_characters(char c) {
        char match_characters[sizeof...(CharacterList)] = { CharacterList... };
        for(int i = 0; i < sizeof...(CharacterList); ++i) {
            if(c == match_characters[i]) {
                return true;
            }
        }
        return false;
    }
    
    template<char ... CharacterList>
    inline void strip_characters(std::string & str) {
        str.erase(std::remove_if(str.begin(), str.end(), &check_characters<CharacterList...>), str.end());
    }
    
    inline std::string alphaNum(std::string s) {
        s.erase(std::remove_if(s.begin(), s.end(), std::not1(std::function<int(int)>((int(*)(int))std::isalnum))), s.end());
        return s;
    }
}
