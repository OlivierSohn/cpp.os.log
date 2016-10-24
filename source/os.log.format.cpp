#include "os.log.format.h"
#include "os.log.h"

namespace imajuscule
{
    void FormatDate(tm*time, std::string&oDate)
    {
        if (likely(time))
        {
            int zero = 0;
            int day = time->tm_mday;
            int month = time->tm_mon + 1;
            int year = time->tm_year + 1900;
            auto sZero = std::to_string(zero);
            auto sDay = std::to_string(day);
            auto sMonth = std::to_string(month);
            auto sYear = std::to_string(year);
            auto sHour = std::to_string(time->tm_hour);
            auto sMinute = std::to_string(time->tm_min);
            auto sSecond = std::to_string(time->tm_sec);
            
            oDate.append(sYear.c_str());
            
            oDate.append("/");
            
            if (month < 10)
                oDate.append(sZero.c_str());
            oDate.append(sMonth.c_str());
            
            oDate.append("/");
            
            if (day < 10)
                oDate.append(sZero.c_str());
            oDate.append(sDay.c_str());
            
            oDate.append(" ");
            
            if (time->tm_hour < 10)
                oDate.append(sZero.c_str());
            oDate.append(sHour.c_str());
            oDate.append(":");
            if (time->tm_min < 10)
                oDate.append(sZero.c_str());
            oDate.append(sMinute.c_str());
            oDate.append(":");
            if (time->tm_sec < 10)
                oDate.append(sZero.c_str());
            oDate.append(sSecond.c_str());
        }
        else
        {
            LG(ERR, "FormatDate: NULL parameter time");
            oDate.assign("..../../.. ..:..:..");
        }
    }
    
    void FormatDateForComparison(std::string & date)
    {
        const char * numbers = "0123456789";
        
        if (11 < date.size())
        {
            if (2 == date.find_first_not_of(numbers, 0))
            {
                if (5 == date.find_first_not_of(numbers, 3))
                {
                    if (10 == date.find_first_not_of(numbers, 6))
                    {
                        //date is with format "dd?mm?yyyy?....." : reverse it
                        std::string newDate;
                        newDate.append(date.substr(6, 4));
                        newDate.append("/");
                        newDate.append(date.substr(3, 2));
                        newDate.append("/");
                        newDate.append(date.substr(0, 2));
                        
                        newDate.append(date.substr(10));
                        
                        A(newDate.size() == date.size());
                        
                        date.swap(newDate);
                    }
                }
            }
        }
    }
    
    void split_in_lines(const std::string &s, char delim, std::vector<std::string> &elems, postProcessing pp) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            if(pp==TRIMMED) {
                trim(item);
            }
            elems.push_back(item);
        }
        if ( s.empty() ) {
            return;
        }
        // add an empty line if new line is the last character
        if(s.back()==delim) {
            elems.emplace_back("");
        }
    }
    std::vector<std::string> split_in_lines(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split_in_lines(s, delim, elems);
        return elems;
    }
    
    inline void add(std::vector<std::string> & vec, std::string && s, postProcessing pp) {
        if(pp == postProcessing::TRIMMED) {
            trim(s);
            if(s.empty()) {
                return;
            }
        }
        vec.emplace_back(s);
    }
    std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters, postProcessing pp)
    {
        using sz = std::string::size_type;
        auto const end = std::string::npos;
        
        std::vector<std::string> tokens;
        
        sz lastPos = str.find_first_not_of(delimiters, 0);
        sz pos = str.find_first_of(delimiters, lastPos);
        
        while ( pos != end || lastPos != end)
        {
            // Found a token, add it to the vector.
            add(tokens, str.substr(lastPos, pos - lastPos), pp);
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
        
        return tokens;
    }
    
    std::vector<std::string> TokenizeMulti(const std::string& str, const std::string& delimiter, postProcessing pp)
    {
        std::vector<std::string> tokens;
        if(delimiter.empty()) {
            for(auto c : str) {
                tokens.emplace_back(1, c);
            }
            return tokens;
        }
        
        using sz = std::string::size_type;
        auto const end = std::string::npos;
        
        sz lastPos = 0;
        while (str.compare(lastPos, delimiter.size(), delimiter) == 0)
        {
            // str begins with delimiter
            lastPos += delimiter.size();
        }
        if(lastPos == str.size()) {
            return tokens;
        }
        
        // lastPos is the first position where we have NOT a delimiter
        
        sz pos = str.find(delimiter, lastPos);
        
        // pos is the position AFTER lastPos where we have a delimiter
        
        A(pos >lastPos);
        while ( pos != end || lastPos != end)
        {
            A(pos >lastPos);
            // Found a token, add it to the vector.
            add(tokens, str.substr(lastPos, pos - lastPos), pp);
            if(pos == end) {
                return tokens;
            }
            lastPos = pos + delimiter.size();
            while (str.compare(lastPos, delimiter.size(), delimiter) == 0)
            {
                lastPos += delimiter.size();
            }
            if(lastPos == str.size()) {
                return tokens;
            }
            
            pos = str.find(delimiter, lastPos);
        }
        
        return tokens;
    }
    

    bool iequals(const std::string& a, const std::string& b, int nCharacters)
    {
        int sz = (int)a.size();
        if(nCharacters >= 0) {
            if((int)b.size() < nCharacters || sz < nCharacters) {
                return false;
            }
        } else {
            if ((int)b.size() != sz) {
                return false;
            }
        }
        for (auto i = 0; (nCharacters < 0 && i < sz) || i<nCharacters; ++i) {
            if (tolower(a[i]) != tolower(b[i])) {
                return false;
            }            
        }
        return true;
    }

    void removeOutterParenthesis(std::string & s) {
        
        while(1) {
            bool found = false;
            
            for(int i=0; i<(int)s.size(); i++) {
                if(s[i] == '(') {
                    int i2;
                    if(findCorrespondantLocation(s, '(', i, ')', true, i2)) {
                        // we found a matching closing parenthesis ...
                        assert(s[i2] == ')');
                        for(int i3 = i2+1; i3<(int)s.size(); ++i3) {
                            if(!std::isspace(s[i3])) {
                                // ... but there is significant information after this parenthesis
                                // so we don't do anything
                                return;
                            }
                        }
                        // ... and there is nothing significant after so we remove both parenthesis
                        found = true;
                        s = s.substr(i+1, i2 - i - 1);
                    }
                    break;
                }
                if(!std::isspace(s[i])) {
                    return;
                }
            }
            
            if(!found) {
                return;
            }
        }
    }

    
    bool findCorrespondantLocation(std::string const & text, const char c1, const int index1, const char c2, const bool bForward, int & index2)
    {
        int length = (int) text.length();
        int i = index1;
        
        assert(i < length);
        assert(text[i] == c1);
        
        int countInBetween = 1;
        for (;;)
        {
            // 1. in(de)crement iterator and break loop if it is out of bounds
            
            if (bForward)
            {
                ++i;
                if (i >= length)
                    break;
            }
            else
            {
                if(0==i)
                    break;
                --i;
            }
            
            // 2. the new iterator is valid
            
            char c = text[i];
            
            // 3. update "countInBetween" according to found char
            
            if (c == c1)
            {
                ++countInBetween;
            }
            else if (c == c2)
            {
                --countInBetween;
            }
            
            if (0 == countInBetween)
            {
                // found it
                index2 = i;
                return true;
            }
        }
        
        return false;
    }
    
    
    bool canCorrespond(const char c, char &cCorrespondant, bool & bForward)
    {
        bool bRet = true;
        bForward = true;
        
        switch (c)
        {
            case '(':
                cCorrespondant = ')';
                break;
            case '[':
                cCorrespondant = ']';
                break;
            case '{':
                cCorrespondant = '}';
                break;
            default:
                bRet = false;
        }
        
        if (!bRet)
        {
            bRet = true;
            bForward = false;
            
            switch (c)
            {
                case ')':
                    cCorrespondant = '(';
                    break;
                case ']':
                    cCorrespondant = '[';
                    break;
                case '}':
                    cCorrespondant = '{';
                    break;
                default:
                    bRet = false;
            }
        }
        
        return bRet;
    }
}

