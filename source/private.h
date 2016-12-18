#include "public.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifdef _WIN32
#include <process.h>
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
