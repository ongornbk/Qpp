// windows.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "windows.h"
#include "window.h"





extern "C"
{
	long start(PointersManager* arg)
	{
		ptrs = arg;
		if (!ptrs) return 1;

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
		lua_pushinteger(state, ptrs->x32(GetForegroundWindow()));
		return 1;
	}

	static int32_t _cdecl _lua_show(lua_State* state)
	{
		lua_pushboolean(state, ShowWindow((HWND)lua_tointeger(state, 1),(int)lua_tointeger(state,2)));
		return 1;
	}

	static int32_t _cdecl _lua_setactive(lua_State* state)
	{
		HWND previous = SetActiveWindow((HWND)lua_tointeger(state, 1));
		lua_pushinteger(state,(lua_Integer)previous);
		return 1;
	}

	static int32_t _cdecl _lua_setfocus(lua_State* state)
	{
		HWND previous = SetFocus((HWND)lua_tointeger(state, 1));
		lua_pushinteger(state,(lua_Integer)previous);
		return 1;
	}

	static int32_t _cdecl _lua_setforeground(lua_State* state)
	{
		lua_pushboolean(state,(SetForegroundWindow((HWND)lua_tointeger(state, 1))));
		return 1;
	}
	
	static int32_t _cdecl _lua_getrect(lua_State* state)
	{
		RECT rect;
		GetWindowRect((HWND)lua_tointeger(state, 1),&rect);
		lua_pushinteger(state, rect.bottom);
		lua_pushinteger(state, rect.left);
		lua_pushinteger(state, rect.right);
		lua_pushinteger(state, rect.top);
		return 4;
	}

	static int32_t _cdecl _lua_validaterect(lua_State* state)
	{
		ValidateRect((HWND)lua_tointeger(state, 1), 0);
		return 0;
	}
	
	static int32_t _cdecl _lua_invalidaterect(lua_State* state)
	{
		InvalidateRect((HWND)lua_tointeger(state, 1), 0,false);
		return 0;
	}
	
	static int32_t _cdecl _lua_settimer(lua_State* state)
	{
		SetTimer((HWND)lua_tointeger(state, 1), (UINT)lua_tointeger(state, 3),(UINT)lua_tointeger(state,2), 0);
		return 0;
	}
	
	static int32_t _cdecl _lua_killtimer(lua_State* state)
	{
		KillTimer((HWND)lua_tointeger(state, 1), lua_tointeger(state, 2));
		return 0;
	}

	static int32_t _cdecl _lua_keypressed(lua_State* state)
	{
		lua_pushboolean(state, ((GetKeyState((int)lua_tointeger(state, 1)) & (short)0x8000) != 0));
		return 1;
	}
	
	static int32_t _cdecl _lua_keydown(lua_State* state)
	{
		lua_pushboolean(state, ((GetKeyState((int)lua_tointeger(state, 1)) & (short)0x100) != 0));
		return 1;
	}

	static int32_t _cdecl _lua_getprocessid(lua_State* state)
	{
		DWORD PID;
		::GetWindowThreadProcessId((HWND)lua_tointeger(state, 1), &PID);
		lua_pushinteger(state, (lua_Integer)PID);
		return 1;
	}

	static int32_t _cdecl _lua_getname(lua_State* state)
	{
		char name[100];
		GetWindowTextA((HWND)lua_tointeger(state, 1), name, 100);
		lua_pushstring(state, name);
		return 1;
	}
	
	static int32_t _cdecl _lua_getclassname(lua_State* state)
	{
		char name[100];
		GetClassNameA((HWND)lua_tointeger(state, 1), name, 100);
		lua_pushstring(state, name);
		return 1;
	}
	
	static int32_t _cdecl _lua_proc(lua_State* state)
	{
		WindowProcedure((HWND)lua_tointeger(state, 1), (uint32_t)lua_tointeger(state, 2), (WPARAM)lua_tointeger(state, 3), (LPARAM)lua_tointeger(state, 4));
		return 0;
		
	}
	
	static int32_t _cdecl _lua_dispatchmessage(lua_State* state)
	{
		DispatchMessage((MSG*)lua_tointeger(state, 1));
		return 0;
	}
	
	static int32_t _cdecl _lua_translatemessage(lua_State* state)
	{
		TranslateMessage((MSG*)lua_tointeger(state, 1));
		return 0;
	}

	static int32_t _cdecl _lua_postquitmessage(lua_State* state)
	{
		PostQuitMessage(0);
		return 0;
	}

	static int32_t _cdecl _lua_getmessage(lua_State* state)
	{
		lua_pushboolean(state, GetMessageA((MSG*)lua_tointeger(state, 2), (HWND)lua_tointeger(state, 1), NULL, NULL));
		return 1;
	}

	static int32_t _cdecl _lua_retrievemessage(lua_State* state)
	{
		MSG* msg = (MSG*)lua_tointeger(state, 1);
		lua_pushinteger(state, (lua_Integer)msg->lParam);
		lua_pushinteger(state, (lua_Integer)msg->wParam);
		lua_pushinteger(state, (lua_Integer)msg->message);
		return 3;
	}

	static int32_t _cdecl _lua_createwindow(lua_State* state)
	{
		Window* window = new Window();
		lua_pushinteger(state,(lua_Integer)window);
		return 1;
	}

	static int32_t _cdecl _lua_destroywindow(lua_State* state)
	{
		Window* window = (Window*)lua_tointeger(state, 1);
		if (window)
		{
			delete window;
			lua_pushboolean(state, true);
		}
		lua_pushboolean(state, false);
		return 1;
	}

	static int32_t _cdecl _lua_getwindowhandle(lua_State* state)
	{
		Window* window = (Window*)lua_tointeger(state, 1);
		if (window) lua_pushinteger(state, (lua_Integer)window->window);
		else lua_pushinteger(state, 0);

		return 1;
	}

	static int32_t _cdecl _lua_update(lua_State* state)
	{
		UpdateWindow((HWND)lua_tointeger(state, 1));
		return 0;
	}
	
	static int32_t _cdecl _lua_settitle(lua_State* state)
	{
		SetWindowTextA((HWND)lua_tointeger(state, 1),lua_tostring(state,2));
		return 0;
	}

	static int32_t _cdecl _lua_getdesktop(lua_State* state)
	{
		lua_pushinteger(state, (lua_Integer)GetDesktopWindow());
		return 1;
	}

	constexpr long FOO_COUNT = 29;

	const char* sckeys[FOO_COUNT] = {
		"BlockInput",
		"CursorPosition",
		"CreateWindow",
		"DestroyWindow",
		"DispatchMessage",
		"GetClassName",
		"GetDesktop",
		"GetForeground",
		"GetMessage",
		"GetName",
		"GetProcessId",
		"GetRect",
		"GetWindowHandle",
		"InvalidateRect",
		"KeyDown",
		"KeyPressed",
		"KillTimer",
		"PostQuitMessage",
		"Proc",
		"RetrieveMessage",
		"SetActive",
		"SetFocus",
		"SetForeground",
		"SetTimer",
		"SetTitle",
		"TranslateMessage",
		"Show",
		"Update",
		"ValidateRect"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		_lua_blockinput,
		_lua_cursorposition,
		_lua_createwindow,
		_lua_destroywindow,
		_lua_dispatchmessage,
		_lua_getclassname,
		_lua_getdesktop,
		_lua_getforegroundwindow,
		_lua_getmessage,
		_lua_getname,
		_lua_getprocessid,
		_lua_getrect,
		_lua_getwindowhandle,
		_lua_invalidaterect,
		_lua_keydown,
		_lua_keypressed,
		_lua_killtimer,
		_lua_postquitmessage,
		_lua_proc,
		_lua_retrievemessage,
		_lua_setactive,
		_lua_setfocus,
		_lua_setforeground,
		_lua_settimer,
		_lua_settitle,
		_lua_translatemessage,
		_lua_show,
		_lua_update,
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

