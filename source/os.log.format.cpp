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
    
    void split_in_lines(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
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
    
    std::vector<std::string> Tokenize(const std::string& str, const std::string& delimiters)
    {
        std::vector<std::string> tokens;
        // Skip delimiters at beginning.
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter".
        std::string::size_type pos = str.find_first_of(delimiters, lastPos);
        
        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        }
        
        return tokens;
    }
    

    bool iequals(const std::string& a, const std::string& b)
    {
        size_t sz = a.size();
        if (b.size() != sz) {
            return false;
        }
        for (size_t i = 0; i < sz; ++i) {
            if (tolower(a[i]) != tolower(b[i])) {
                return false;
            }            
        }
        return true;
    }

}

