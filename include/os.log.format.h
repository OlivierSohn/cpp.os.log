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
#include <cassert>

namespace imajuscule
{
    enum postProcessing {
        TRIMMED,
        NOT_TRIMMED
    };
    void split_in_lines(const std::string &s, char delim, std::vector<std::string> &elems, postProcessing pp = NOT_TRIMMED);
    std::vector<std::string> split_in_lines(const std::string &s, char delim = '\n');
    std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters = " ", postProcessing pp = NOT_TRIMMED);
    std::vector<std::string> TokenizeMulti(const std::string& str, const std::string& delimiter, postProcessing pp = NOT_TRIMMED);
    
    void FormatDate(tm*time, std::string&oDate);
    void FormatDateForComparison(std::string & date);
    bool iequals(const std::string& a, const std::string& b, int nChars = -1);
    
    
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
        auto tmp = std::move(begin);
        auto size_comparison = (int)tmp.size();
        return iequals(tmp, s, size_comparison) ? size_comparison : 0;
    }
    inline int begins_with(std::string const& s, std::string const & begin) {
        return begins_with(s,std::string(begin));
    }
    
    
    // trim from start (in place)
    inline int ltrim(std::string &s) {
        auto beg = s.begin();
        auto first_non_space = std::find_if(beg, s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
        int range = static_cast<int>(std::distance(beg, first_non_space));
        s.erase(beg, first_non_space);
        return range;
    }
    
    // trim from end (in place)
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    }

    // trim from start (in place)
    inline bool ltrim(std::string &s, char c, int maxCount = -1) {
        int i=0;
        int size = (int)s.size();
        while(i!= maxCount && i < size) {
            if(!std::isspace(s[i]) && s[i] != c) {
                break;
            }
            ++i;
        }
        if(!i) {
            return false;
        }
        s.erase(0, i);
        return true;
    }
    
    // trim from end (in place)
    inline bool rtrim(std::string &s, char c, int maxCount = -1) {
        int size = (int)s.size();
        int i = size - 1;
        int n=0;
        while(n!= maxCount && i >= 0) {
            if(!std::isspace(s[i]) && s[i] != c) {
                break;
            }
            --i;
            ++n;
        }
        if(!n) {
            return false;
        }
        s.erase(i+1, n);
        return true;
    }

    // trim from both ends (in place)
    // returns the number of characters removed on the left side
    inline int trim(std::string &s) {
        rtrim(s);
        return ltrim(s);
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
    
    inline bool toFloat(std::string const & str, float & f) {
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
    
    static inline void Lower( std::string & s) {
        for (auto & c : s) {
            c = std::tolower(c);
        }
    }
    static inline void Upper( std::string & s) {
        for (auto & c : s) {
            c = std::toupper(c);
        }
    }
    static inline std::string toLower( std::string s ) {
        Lower(s);
        return s;
    }
    static inline std::string toUpper( std::string s ) {
        Upper(s);
        return s;
    }
    
    bool findCorrespondantLocation(std::string const & text, const char c1, const int index1, const char c2, const bool bForward, int & index2);
    bool canCorrespond(const char c, char &cCorrespondant, bool & bForward);
    
    void removeOutterParenthesis(std::string & s);
    
    static inline std::vector<std::string> variables(std::string const & str) {
        auto str_ = str;
        removeOutterParenthesis(str_);
        auto v = Tokenize(str_,",", postProcessing::TRIMMED);
        v.erase(std::remove(v.begin(), v.end(), ""), v.end());
        return v;
    }
    
    static inline bool before_after(std::string & input_then_before, std::string delimiter, std::string & after) {
        auto v = TokenizeMulti(input_then_before, delimiter, TRIMMED);
        if(v.size() != 2) {
            return false;
        }
        input_then_before = v[0];
        after = v[1];
        return true;
    }
}
