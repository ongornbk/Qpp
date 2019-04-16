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

	static int32_t _cdecl _lua_setactive(lua_State* state)
	{
		ptrtype lptr(SetActiveWindow((HWND)ptrtype(lua_tointeger(state,1),lua_tointeger(state,2)).ptr));
		lua_pushinteger(state, lptr.lua.first);
		lua_pushinteger(state, lptr.lua.second);
		return 2;
	}

	static int32_t _cdecl _lua_setfocus(lua_State* state)
	{
		ptrtype lptr(SetFocus((HWND)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).ptr));
		lua_pushinteger(state, lptr.lua.first);
		lua_pushinteger(state, lptr.lua.second);
		return 2;
	}

	static int32_t _cdecl _lua_setforeground(lua_State* state)
	{
		lua_pushboolean(state,(SetForegroundWindow((HWND)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).ptr)));
		return 1;
	}
	
	static int32_t _cdecl _lua_getrect(lua_State* state)
	{
		RECT rect;
		GetWindowRect((HWND)ptrtype(lua_tointeger(state,1),lua_tointeger(state,2)).ptr,&rect);
		lua_pushinteger(state, rect.bottom);
		lua_pushinteger(state, rect.left);
		lua_pushinteger(state, rect.right);
		lua_pushinteger(state, rect.top);
		return 4;
	}

	static int32_t _cdecl _lua_validaterect(lua_State* state)
	{
		ValidateRect((HWND)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).ptr, 0);
		return 0;
	}
	
	static int32_t _cdecl _lua_invalidaterect(lua_State* state)
	{
		InvalidateRect((HWND)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).ptr, 0,false);
		return 0;
	}
	
	static int32_t _cdecl _lua_settimer(lua_State* state)
	{
		SetTimer((HWND)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).ptr, lua_tointeger(state, 4),lua_tointeger(state,3), 0);
		return 0;
	}
	
	static int32_t _cdecl _lua_killtimer(lua_State* state)
	{
		KillTimer((HWND)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).ptr, lua_tointeger(state, 3));
		return 0;
	}

	static int32_t _cdecl _lua_keypressed(lua_State* state)
	{
		lua_pushboolean(state, (GetKeyState(lua_tointeger(state, 1)) & 0x8000 != 0));
		return 1;
	}
	
	static int32_t _cdecl _lua_keydown(lua_State* state)
	{
		lua_pushboolean(state, ((GetKeyState(lua_tointeger(state, 1)) & 0x100) != 0));
		return 1;
	}

	static int32_t _cdecl _lua_exist(lua_State* state)
	{
		lua_pushboolean(state,(bool)ptrtype(lua_tointeger(state, 1), lua_tointeger(state, 2)).val);
		return 0;
	}

	constexpr long FOO_COUNT = 15;

	const char* sckeys[FOO_COUNT] = {
		"BlockInput",
		"CursorPosition",
		"Exist",
		"GetForeground",
		"GetRect",
		"InvalidateRect",
		"KeyDown",
		"KeyPressed",
		"KillTimer",
		"SetActive",
		"SetFocus",
		"SetForeground",
		"SetTimer",
		"Show",
		"ValidateRect"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		_lua_blockinput,
		_lua_cursorposition,
		_lua_exist,
		_lua_getforegroundwindow,
		_lua_getrect,
		_lua_invalidaterect,
		_lua_keydown,
		_lua_keypressed,
		_lua_killtimer,
		_lua_setactive,
		_lua_setfocus,
		_lua_setforeground,
		_lua_settimer,
		_lua_show,
		_lua_validaterect
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

