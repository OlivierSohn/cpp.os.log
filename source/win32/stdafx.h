// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <tchar.h>

#include <ctime>
#include <string>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "DbgHelp.h"
/*#include <WinBase.h>*/
#include <process.h>
#include <iostream>
#endif

// this module
#include "os.log.allincludes.h"
