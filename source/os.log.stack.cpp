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

#endif

#ifdef FILE_PATH_PATTERN
#error 0
#else
#define FILE_PATH_PATTERN FILE_SEPARATOR "olivier" FILE_SEPARATOR "dev" FILE_SEPARATOR
#endif

namespace imajuscule
{
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
	IMAGEHLP_LINE64 *Line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
	Line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	char * id = FILE_PATH_PATTERN;

	int sizeSymbol(0);
	int sizeFile(0);
	const int sizeSymbolMax = 50;
	const int sizeFileMax = 50;
	for (int i = 0; i < numberOfFrames; i++)
	{
		DWORD64 address = (DWORD64)(stack[i]);
		SymFromAddr(process, address, NULL, symbol);
		if (SymGetLineFromAddr64(process, address, &displacement, Line))
		{
			char * file = Line->FileName;
			if (char* token = strstr(Line->FileName, id))
				file = token + strlen(id);

			int locSizeFile = strlen(file);
			if (locSizeFile <= sizeFileMax)
				sizeFile = std::max(sizeFile, locSizeFile);
		}
		int locSizeSymbol = strlen(symbol->Name);
		if(locSizeSymbol <= sizeSymbolMax)
			sizeSymbol = std::max(sizeSymbol, locSizeSymbol);
	}

	for (int i = 1/*skip current method*/; i < numberOfFrames; i++)
	{
		DWORD64 address = (DWORD64)(stack[i]);
		SymFromAddr(process, address, NULL, symbol);
		if (SymGetLineFromAddr64(process, address, &displacement, Line))
		{
			char * file = Line->FileName;
			if (char* token = strstr(Line->FileName, id))
				file = token + strlen(id);

			int place, place2;
			LG(INFO, "%#16I64x%n|%-*s%s%n|%-*s%s|%lu", 
				symbol->Address, 
				&place, 
				sizeSymbol, symbol->Name,
				(strlen(symbol->Name) <= sizeSymbolMax) ? "" : ("\n" + std::string(place + sizeSymbol + 1 + 5, ' ')).c_str(),
				&place2,
				sizeFile, file,
				(strlen(file) <= sizeFileMax) ? "" : ("\n" + std::string(place2 + sizeFile + 1 + 5, ' ')).c_str(),
				Line->LineNumber);
		}
		else
		{
			//LG(INFO, "\tSymGetLineFromAddr64 returned error code %lu", GetLastError());
			LG(INFO, "%#16I64x|%-*s", symbol->Address, sizeSymbol, symbol->Name);
		}
	}

#else

	const Integer SIZE = 100;
	void *buffer[SIZE];
	char **strings;

	int nStrings = backtrace(buffer, SIZE);

	strings = backtrace_symbols(buffer, nStrings);
	if (!strings)
	{
		LG(ERR,"logStack: backtrace_symbols returned NULL");
		return;
	}

	for (auto s = 1/*skip current method*/; s < nStrings; ++s)
	{
		LG(INFO,"%s", strings[s]);
	}

	free(strings);

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