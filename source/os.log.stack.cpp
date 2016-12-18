

#ifdef FILE_PATH_PATTERN
#error 0
#else
#define FILE_PATH_PATTERN FILE_SEPARATOR "olivier" FILE_SEPARATOR "dev" FILE_SEPARATOR
#endif

namespace imajuscule
{
	static void simplifySymbol(std::string & subTrace)
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

#ifdef _WIN32
    static void CreateMiniDump(EXCEPTION_POINTERS* pep)
    {
        // Open the file 

        LG(ERR, "writing minidump...");
        HANDLE hFile = CreateFile(L"MiniDump.dmp", GENERIC_READ | GENERIC_WRITE,
            0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

        if ( (hFile != nullptr) && (hFile != INVALID_HANDLE_VALUE) )
        {
            // Create the minidump 

            MINIDUMP_EXCEPTION_INFORMATION mdei;

            mdei.ThreadId = GetCurrentThreadId();
            mdei.ExceptionPointers = pep;
            mdei.ClientPointers = FALSE;

            MINIDUMP_TYPE mdt = MiniDumpNormal;

            BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0);

            if ( !rv )
                LG(ERR,"MiniDumpWriteDump failed. Error: %u", GetLastError());

            // Close the file 

            CloseHandle(hFile);

        } else
        {
            LG(ERR, "CreateFile failed. Error: %u", GetLastError());
        }

    }
#endif

    void logStack()
{
#ifdef _WIN32
    CreateMiniDump(0);
    return;
#else
    
    const char * line = "   o___.__o______o_____._______________o___o_____._________o_o______._o__._________o_.______.___o_o";
    const char * lin = "   o                                                                                              o";
    const char * lineS = " o  ______________________________________________________________________________________________  o";
    const char * pipes = " o                                                                                                  o";
    const char * pipes2 = "    o                                                                                            o";
    const char * Header = "O                                          STACK BEGIN                                               O";
    const char * Footer = "O           .                              STACK END          o                .                     O";
    const char * pip = "O                                                                                                    O";
    const char * pipesU = "o __________________________________________________________________________________________________ o";
    const char * st = " o                                                                                                  o";
    const char * pipesS = " O ___ o ____ . ____ o ________________._________________ O _______________ o _________._____ o ___ O ";

    LG(INFO, "");
    LG(INFO, line);
    LG(INFO, pipes);
    LG(INFO, Header);
    LG(INFO, pipesU);
    LG(INFO, st);
    LG(INFO, "");

    const int max_frames(100);
    
    // storage array for stack trace address data
    void* addrlist[max_frames+1];
    
    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    if ( unlikely(!addrlen))
    {
        LG(ERR,"logStack: backtrace_symbols returned nullptr");
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
                        abi::__cxa_demangle(subTrace.c_str(), nullptr, nullptr, &demangleStatus),
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

    LG(INFO, "");
    LG(INFO, pipes2);
    LG(INFO, pipesS);
    LG(INFO, pip);
    LG(INFO, Footer);
    LG(INFO, lineS);
    LG(INFO, lin);
    LG(INFO, "");
#endif

	}
}
