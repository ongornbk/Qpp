// filesystem.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "filesystem.h"

extern "C"
{
	long start(const long arg)
	{

		return 0;
	}

	long close(const long arg)
	{

		return 0;
	}

	static int32_t _cdecl _lua_mkdir(lua_State* state)
	{
		lua_pushboolean(state, CreateDirectoryA(lua_tostring(state, 1), NULL));
		return 1;
	}

	static int32_t _cdecl _lua_getenv(lua_State* state)
	{
#pragma warning(disable : 4996)
		lua_pushstring(state, getenv(lua_tostring(state, 1)));
		return 1;
	}

	constexpr long FOO_COUNT = 2;

	const char* sckeys[FOO_COUNT] = {
	"GetEnv",
	"Mkdir"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
	_lua_getenv,
	_lua_mkdir
	};

	long foo_count(const long arg)
	{
		return FOO_COUNT;
	}

	cpair get_foo(const long index)
	{
		cpair cp;
		cp.key = sckeys[index];
		cp.value = scfooes[index];
		return cp;
	}
}

