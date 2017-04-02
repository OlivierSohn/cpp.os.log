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

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#endif
