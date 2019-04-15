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
#include <stdlib.h>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <sal.h>

#if _WIN64 
#define CALL_CONV __stdcall
#pragma comment(lib,"slib/x64/liblua53.a")
#else
#define CALL_CONV __cdecl
#pragma comment(lib,"slib/x86/liblua53.a")
#endif

typedef int32_t(*lua_CFunction) (lua_State *L);

struct lua64uint
{
	explicit lua64uint(const _In_ uint32_t f, const uint32_t s) : first(f), second(s) {}

	uint32_t first;
	uint32_t second;
};

union ptrtype
{
	ptrtype(void* v)
	{
		ptr = v;
	}
	ptrtype(uint32_t first, uint32_t second)
	{
		lua = lua64uint(first, second);
	}
	ptrtype(uint64_t i)
	{
		val = i;
	}
	ptrtype(lua64uint l)
	{
		lua = l;
	}
	__unaligned void* ptr;
	uint64_t          val;
	lua64uint         lua;
};

// reference additional headers your program requires here
