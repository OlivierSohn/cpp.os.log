#include "format.h"
#include "os.log.format.h"
#include "os.log.h"
namespace imajuscule
{
    void FormatDate(tm*time, std::string&oDate)
    {
        //LG(INFO, "FormatDate begin");
        
        if (likely(time))
        {
            int zero = 0;
            int day = time->tm_mday;
            int month = time->tm_mon + 1;
            int year = time->tm_year + 1900;
            fmt::FormatInt sZero(zero);
            fmt::FormatInt sDay(day);
            fmt::FormatInt sMonth(month);
            fmt::FormatInt sYear(year);
            fmt::FormatInt sHour(time->tm_hour);
            fmt::FormatInt sMinute(time->tm_min);
            fmt::FormatInt sSecond(time->tm_sec);
            
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
            
            //LG(INFO, "FormatDate: \"%s\"", oDate.c_str());
        }
        else
        {
            LG(ERR, "FormatDate: NULL parameter time");
            oDate.assign("..../../.. ..:..:..");
        }
        
        //LG(INFO, "FormatDate end");
    }
    
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        // add an empty line if new line is the last character
        if(s.back()==delim)
            elems.emplace_back("");
        
        return elems;
    }
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
    
    bool iequals(const std::string& a, const std::string& b)
    {
        size_t sz = a.size();
        if (b.size() != sz)
            return false;
        for (size_t i = 0; i < sz; ++i)
            if (tolower(a[i]) != tolower(b[i]))
                return false;
        return true;
    }

}

