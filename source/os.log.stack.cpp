

namespace imajuscule
{
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
    
    const char * line =   "   o___.__o______o_____._______________o___o_____._________o_o______._o__._________o_.______.___o_o";
    const char * lin =    "   o                                                                                              o";
    const char * lineS =  " o  ______________________________________________________________________________________________  o";
    const char * pipes =  " o                                                                                                  o";
    const char * pipes2 = "    o                                                                                            o";
    const char * Header = "O                                          STACK BEGIN                                               O";
    const char * Footer = "O           .                              STACK END          o                .                     O";
    const char * pip =    "O                                                                                                    O";
    const char * pipesU = "o __________________________________________________________________________________________________ o";
    const char * st =     " o                                                                                                  o";
    const char * pipesS = " O ___ o ____ . ____ o ________________._________________ O _______________ o _________._____ o ___ O ";

    std::cout << std::endl
    << line << std::endl
    << pipes << std::endl
    << Header << std::endl
    << pipesU << std::endl
    << st << std::endl << std::endl;
    
    constexpr auto n_remove = 1; // for this function
    for(auto & t : debugging::getProgramStack(n_remove)) {
        std::cout << t << std::endl;
    }
    
    std::cout << std::endl
    << pipes2 << std::endl
    << pipesS << std::endl
    << pip << std::endl
    << Footer << std::endl
    << lineS << std::endl
    << lin << std::endl << std::endl;
    
#endif

	}
}
