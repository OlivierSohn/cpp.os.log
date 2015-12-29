#ifdef _WIN32
#include <algorithm>
#include <process.h>
#include <iostream>
#define NOMINMAX
#include <Windows.h>
#include "dbghelp.h"
#pragma comment(lib, "Dbghelp.lib")

#ifdef FILE_SEPARATOR
#error 0
#else
#define FILE_SEPARATOR "\\"
#endif

#else

#ifdef FILE_SEPARATOR
#error 0
#else
#define FILE_SEPARATOR "/"
#endif

#include <cstdlib>
#include <execinfo.h>
#include <cxxabi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#endif

#include "os.log.h"
#include <string>

#ifdef FILE_PATH_PATTERN
#error 0
#else
#define FILE_PATH_PATTERN FILE_SEPARATOR "olivier" FILE_SEPARATOR "dev" FILE_SEPARATOR
#endif

namespace imajuscule
{
	void simplifySymbol(std::string & subTrace)
	{
		std::string s = "imajuscule::";

		bool bGoOn(false);
		do
		{
			std::string::size_type i = subTrace.find(s);
			bGoOn = (i != std::string::npos);
			if (bGoOn)
				subTrace.erase(i, s.length());
		} while (bGoOn);
	}

void logStack()
{
	const char * line =   "   o___.__o______o_____._______________o___o_____._________o_o______._o__._________o_.______.___o_o";
	const char * lin =    "   o                                                                                              o";
	const char * lineS =  " o  ______________________________________________________________________________________________  o";
	const char * pipes =  " o                                                                                                  o";
	const char * pipes2 = "    o                                                                                            o";
	const char * Header = "O                                          STACK BEGIN                                               O";
	const char * Footer = "O           .                              STACK END          o                .                     O";
	const char * pip =    "O                                                                                                    O";
	const char * pipesU = "o __________________________________________________________________________________________________ o";
	const char * st     = " o                                                                                                  o";
	const char * pipesS = " O ___ o ____ . ____ o ________________._________________ O _______________ o _________._____ o ___ O ";

	LG(INFO, "");
	LG(INFO, line);
	LG(INFO, pipes);
	LG(INFO, Header);
	LG(INFO, pipesU);
	LG(INFO, st);
	LG(INFO, "");

#ifdef _WIN32
	_set_printf_count_output(1);
#define TRACE_MAX_STACK_FRAMES 1024
#define TRACE_MAX_FUNCTION_NAME_LENGTH 1024

	void *stack[TRACE_MAX_STACK_FRAMES];
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	WORD numberOfFrames = CaptureStackBackTrace(0, TRACE_MAX_STACK_FRAMES, stack, NULL);
	SYMBOL_INFO *symbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO) + (TRACE_MAX_FUNCTION_NAME_LENGTH - 1) * sizeof(TCHAR));
	symbol->MaxNameLen = TRACE_MAX_FUNCTION_NAME_LENGTH;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	DWORD displacement;
#ifdef _WIN64
	IMAGEHLP_LINE64 *Line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
	Line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
#else
	IMAGEHLP_LINE *Line = (IMAGEHLP_LINE *)malloc(sizeof(IMAGEHLP_LINE));
	Line->SizeOfStruct = sizeof(IMAGEHLP_LINE);
#endif

	char * id = FILE_PATH_PATTERN;

	int sizeSymbol(0);
	int sizeFile(0);
	const int sizeSymbolMax = 50;
	const int sizeFileMax = 50;
	for (int i = 0; i < numberOfFrames; i++)
	{
#ifdef _WIN64
		DWORD64 address = (DWORD64)(stack[i]);
#else
		DWORD address = (DWORD)(stack[i]);
#endif
		SymFromAddr(process, address, NULL, symbol);
		if (
#ifdef _WIN64
			SymGetLineFromAddr64
#else
			SymGetLineFromAddr
#endif		
			(process, address, &displacement, Line))
		{
			char * file = Line->FileName;
			if (char* token = strstr(Line->FileName, id))
				file = token + strlen(id);

			int locSizeFile = strlen(file);
			if (locSizeFile <= sizeFileMax)
				sizeFile = std::max(sizeFile, locSizeFile);
		}
		std::string subTrace = symbol->Name;
		simplifySymbol(subTrace);
		int locSizeSymbol = subTrace.size();
		if(locSizeSymbol <= sizeSymbolMax)
			sizeSymbol = std::max(sizeSymbol, locSizeSymbol);
	}

	for (int i = 1/*skip current method*/; i < numberOfFrames; i++)
	{
#ifdef _WIN64
		DWORD64 address = (DWORD64)(stack[i]);
#else
		DWORD address = (DWORD)(stack[i]);
#endif		
		SymFromAddr(process, address, NULL, symbol);
		if (
#ifdef _WIN64
			SymGetLineFromAddr64
#else
			SymGetLineFromAddr
#endif		
			(process, address, &displacement, Line))
		{
			char * file = Line->FileName;
			if (char* token = strstr(Line->FileName, id))
				file = token + strlen(id);

			std::string subTrace = symbol->Name;
			simplifySymbol(subTrace);

            const bool bSameLine = (subTrace.size() <= sizeSymbolMax);
            const char * hundredSpaces = "                                                                                                  ";
            const int nSpaces = bSameLine ? 0 : (2 + 4 /*INFO*/+ 16 + sizeSymbol);

            LG(INFO, "%0#16I64X|%-*s%s%.*s|%s (%lu)",
                symbol->Address,
                sizeSymbol, subTrace.c_str(),
                bSameLine ? "" : "\n",
                nSpaces,
                hundredSpaces,
                file,
                Line->LineNumber);
		}
		else
		{
			//LG(INFO, "\tSymGetLineFromAddr64 returned error code %lu", GetLastError());
			LG(INFO, "%#16I64x|%-*s", symbol->Address, sizeSymbol, symbol->Name);
		}
	}

#else
    
    const int max_frames(100);
    
    // storage array for stack trace address data
    void* addrlist[max_frames+1];
    
    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    if ( unlikely(!addrlen))
    {
        LG(ERR,"logStack: backtrace_symbols returned NULL");
        return;
    }
    
    if ( unlikely(addrlen == 0)) {
        LG(ERR,"logStack: trace empty, possibly corrupt");
        return;
    }
    
    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);
    
    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 1024;
    char* funcname = (char*)malloc(funcnamesize);
    
    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
        std::string trace(symbollist[i]);
        
        // parse trace to find symbol, unmangle it
        std::string::size_type loc_ox = trace.find_first_of("0x");
        if(loc_ox != std::string::npos)
        {
            std::string::size_type loc_space = trace.find_first_of(" ", loc_ox);
            if(loc_space != std::string::npos)
            {
                loc_space++;
                std::string::size_type loc_space2 = trace.find_first_of(" ", loc_space);
                if(loc_space2 != std::string::npos)
                {
                    std::string subTrace = trace.substr(loc_space, loc_space2 - loc_space);
                    int demangleStatus;
                    
                    std::unique_ptr<char, void(*)(void*)> res {
                        abi::__cxa_demangle(subTrace.c_str(), NULL, NULL, &demangleStatus),
                        std::free
                    };
                    
                    if( res && demangleStatus == 0)
                    {
                        subTrace = res.get();
                        
						simplifySymbol(subTrace);

                        trace.replace(loc_space, loc_space2 - loc_space, subTrace);
                    }
                }
            }
        }
        
        LG(INFO, "%s", trace.c_str());
    }
    
    free(funcname);
    free(symbollist);

#endif

	LG(INFO, "");
	LG(INFO, pipes2);
	LG(INFO, pipesS);
	LG(INFO, pip);
	LG(INFO, Footer);
	LG(INFO, lineS);
	LG(INFO, lin);
	LG(INFO, "");
	}
}
