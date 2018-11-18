#pragma once
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>




#if _WIN64 
#define CALL_CONV __stdcall
#pragma comment(lib,"slib/x64/liblua53.a")
#else
#define CALL_CONV __cdecl
#pragma comment(lib,"slib/x86/liblua53.a")
#endif

