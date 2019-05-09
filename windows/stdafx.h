// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <cstdint>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <unordered_map>
#include <ctime>
#include <sal.h>

#if _WIN64 
#define CALL_CONV __stdcall
#pragma comment(lib,"slib/x64/liblua53.a")
#else
#define CALL_CONV __cdecl
#pragma comment(lib,"slib/x86/liblua53.a")
#endif

typedef int32_t(*lua_CFunction) (lua_State *L);

// reference additional headers your program requires here
