#pragma once

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>
#include <locale>

namespace imajuscule
{
    void split_in_lines(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split_in_lines(const std::string &s, char delim = '\n');
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
    
    inline std::string opposite( std::string const & s ) {
        return "-(" + s + ")";
    }
    inline std::string opposite( std::string && s ) {
        return "-(" + s + ")";
    }
    
    inline int begins_with(std::string const& s, std::string && begin) {
        return s.compare(0, begin.size(), begin) ? 0: (int) begin.size();
    }
    
    
    // trim from start (in place)
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    }
    
    // trim from end (in place)
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    }

    // trim from start (in place)
    inline bool ltrim(std::string &s, char c) {
        bool result = false;
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [=, &result](char C) {
            if(std::isspace(c)) {
                return false;
            }
            if(c==C) {
                result = true;
                return false;
            }
            return true;
        }));
        return result;
    }
    
    // trim from end (in place)
    inline bool rtrim(std::string &s, char c) {
        bool result = false;
        s.erase(std::find_if(s.rbegin(), s.rend(), [=, &result](char C) {
            if(std::isspace(c)) {
                return false;
            }
            if(c==C) {
                result = true;
                return false;
            }
            return true;
        }).base(), s.end());
        return result;
    }

    // trim from both ends (in place)
    inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
    
    // trim from start (copying)
    inline std::string ltrimmed(std::string s) {
        ltrim(s);
        return s;
    }
    
    // trim from end (copying)
    inline std::string rtrimmed(std::string s) {
        rtrim(s);
        return s;
    }
    
    // trim from both ends (copying)
    inline std::string trimmed(std::string s) {
        trim(s);
        return s;
    }
    
    inline bool removeParenthesis(std::string & str) {
        bool res = ltrim(str,'(');
        return rtrim(str,')') || res;
    }
    
    inline bool toFloat(std::string & str, float & f) {
        trim(str);
        if(str.empty()) {
            return false;
        }
        char * e;
        f = std::strtof(str.c_str(), &e);
        
        if (*e != '\0' // error, we didn't consume the entire string
            )
        {
            return false;
        }
        return true;
    }
    
    static inline std::vector<std::string> variables(std::string const & str) {
        auto v = Tokenize(str,",");
        for(auto & s : v) {
            trim(s);
        }
        v.erase(std::remove(v.begin(), v.end(), ""), v.end());
        return v;
    }
}
