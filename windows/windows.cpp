// windows.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "windows.h"

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

	static int32_t _cdecl _lua_blockinput(lua_State* state)
	{
	lua_pushboolean(state,BlockInput(lua_toboolean(state, 1)));
	return 1;
	}

	static int32_t _cdecl _lua_cursorposition(lua_State* state)
	{
	POINT p;
	GetCursorPos(&p);
	p.y -= 22;
	lua_pushinteger(state, p.x);
	lua_pushinteger(state, p.y);
	return 2;
	}

	static int32_t _cdecl _lua_getforegroundwindow(lua_State* state)
	{
		ptrtype lptr(GetForegroundWindow());
		lua_pushinteger(state, lptr.lua.first);
		lua_pushinteger(state, lptr.lua.second);
		
		return 2;
	}

	static int32_t _cdecl _lua_show(lua_State* state)
	{
		lua_pushboolean(state,ShowWindow((HWND)ptrtype(lua_tointeger(state,1),lua_tointeger(state,2)).ptr, lua_toboolean(state, 3)));
		return 1;
	}

	constexpr long FOO_COUNT = 4;

	const char* sckeys[FOO_COUNT] = {
		"BlockInput",
		"CursorPosition",
		"GetForeground",
		"Show"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		_lua_blockinput,
		_lua_cursorposition,
		_lua_getforegroundwindow,
		_lua_show
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

