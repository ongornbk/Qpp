// windows.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "windows.h"
#include "window.h"
#include "time.h"

namespace
{
	std::unordered_map<uint32_t, std::string> lua_events;
}


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

	static int32_t _cdecl _lua_blockinput(
		struct lua_State* const state
	)
	{
	lua_pushboolean(state,BlockInput(lua_toboolean(state, 1)));
	return 1;
	}

	static int32_t _cdecl _lua_windowfrompoint(
		struct lua_State* const state
	)
	{
		POINT point;
		point.x = (LONG)(lua_tointeger(state, 1));
		point.y = (LONG)(lua_tointeger(state, 2));
		lua_pushinteger(state,(lua_Integer)WindowFromPoint(point));
		return 1;
	}

	static int32_t _cdecl _lua_cursorposition(
		struct lua_State* const state
	)
	{
	POINT p;
	GetCursorPos(&p);
	p.y -= 22;
	lua_pushinteger(state, (lua_Integer)(p.x));
	lua_pushinteger(state, (lua_Integer)(p.y));
	return 2;
	}

	static int32_t _cdecl _lua_getforegroundwindow(lua_State* state)
	{
		lua_pushinteger(state, (lua_Integer)GetForegroundWindow());
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
	
	static int32_t _cdecl _lua_getrect(
		struct lua_State* const state
	)
	{
		RECT rect{};
		GetWindowRect((HWND)lua_tointeger(state, 1),&rect);
		lua_pushinteger(state, rect.bottom);
		lua_pushinteger(state, rect.left);
		lua_pushinteger(state, rect.right);
		lua_pushinteger(state, rect.top);
		return 4;
	}

	static int32_t _cdecl _lua_getclientrect(
		struct lua_State* const state
	)
	{
		RECT rect{};
		GetClientRect((HWND)lua_tointeger(state, 1), &rect);
		lua_pushinteger(state, rect.bottom);
		lua_pushinteger(state, rect.left);
		lua_pushinteger(state, rect.right);
		lua_pushinteger(state, rect.top);
		return 4;
	}

	static int32_t _cdecl _lua_validaterect(
		struct lua_State* const state
	)
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
	
	static int32_t _cdecl _lua_proc(
		struct lua_State* const state
	)
	{
		lua_pushinteger(state,(lua_Integer)WindowProcedure(
			(HWND)lua_tointeger(state, 1),
			(uint32_t)lua_tointeger(state, 2),
			(WPARAM)lua_tointeger(state, 3),
			(LPARAM)lua_tointeger(state, 4)
		));
		return 1;
		
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

	static int32_t _cdecl _lua_peekmessage(lua_State* state)
	{
		lua_pushboolean(state, PeekMessageA((MSG*)lua_tointeger(state, 2), (HWND)lua_tointeger(state, 1), NULL, NULL,NULL));
		return 1;
	}

	static int32_t _cdecl _lua_retrievemessage(
		struct lua_State* const state
	)
	{
		MSG* const msg = (MSG*)lua_tointeger(state, 1);
		lua_pushinteger(state, (lua_Integer)msg->lParam);
		lua_pushinteger(state, (lua_Integer)msg->wParam);
		lua_pushinteger(state, (lua_Integer)msg->message);
		return 3;
	}

	static int32_t _cdecl _lua_createwindow(
		struct lua_State* const state
	)
	{
		class Window* const window = new class Window(state);
		lua_pushinteger(state,(lua_Integer)window);
		return 1;
	}

	static int32_t _cdecl _lua_destroywindow(
		struct lua_State* const state
	)
	{
		class Window* const window = (class Window* const)lua_tointeger(state, 1);
		if (window)
		{
			delete window;
			lua_pushboolean(state, true);
		}
		lua_pushboolean(state, false);
		return 1;
	}

	static int32_t _cdecl _lua_getwindowhandle(
		struct lua_State* const state
	)
	{
		class Window* const window = (class Window* const)lua_tointeger(state, 1);
		if (window) lua_pushinteger(state, (lua_Integer)window->window);
		else lua_pushinteger(state, 0);
		return 1;
	}

	static int32_t _cdecl _lua_update(
		struct lua_State* const state
	)
	{
		UpdateWindow((HWND)lua_tointeger(state, 1));
		return 0;
	}
	
	static int32_t _cdecl _lua_settitle(
		struct lua_State* const state
	)
	{
		SetWindowTextA((HWND)lua_tointeger(state, 1),lua_tostring(state,2));
		return 0;
	}

	static int32_t _cdecl _lua_getdesktop(
		struct lua_State* const state
	)
	{
		lua_pushinteger(state, (lua_Integer)GetDesktopWindow());
		return 1;
	}

	static int32_t _cdecl _lua_sendmessage(
		struct lua_State* const state
	)
	{
		SendMessage((HWND)lua_tointeger(state,1), (UINT)lua_tointeger(state, 2), lua_tointeger(state, 3), MAKELPARAM(lua_tointeger(state, 4), lua_tointeger(state, 5)));
		return 0;
	}

	static int32_t _cdecl _lua_postmessage(
		struct lua_State* const state
	)
	{
		PostMessage((HWND)lua_tointeger(state, 1), (UINT)lua_tointeger(state, 2), lua_tointeger(state, 3), MAKELPARAM(lua_tointeger(state, 4), lua_tointeger(state, 5)));
		return 0;
	}
	
	static int32_t _cdecl _lua_clienttoscreen(
		struct lua_State* const state
	)
	{
		POINT p;
		p.x = (int32_t)lua_tointeger(state, 2);
		p.y = (int32_t)lua_tointeger(state, 3);
		ClientToScreen((HWND)lua_tointeger(state,1), &p);
		lua_pushinteger(state,(lua_Integer)(p.x));
		lua_pushinteger(state,(lua_Integer)(p.y));
		return 2;
	}
	
	static int32_t _cdecl _lua_gettime(lua_State* state)//todo
	{
		//Time t;
		//lua_pushinteger(state,t.)
	}

	static int32_t _cdecl _lua_registerhotkey(lua_State* state)
	{
		RegisterHotKey(NULL, (int)int32_t(lua_tointeger(state, 1)), MOD_NOREPEAT, (UINT)lua_tointeger(state, 2));
		return 1;
	}

	static int32_t _cdecl _lua_getscreenmetrics(lua_State* state)
	{
		lua_pushinteger(state, (lua_Integer)GetSystemMetrics(SM_CXSCREEN));
		lua_pushinteger(state, (lua_Integer)GetSystemMetrics(SM_CYSCREEN));
		return 2;
	}

	static int32_t _cdecl _lua_beginpaint(
		struct lua_State* const state
	)
	{
		HDC const hdc = BeginPaint((HWND)lua_tointeger(state, 1), (PAINTSTRUCT*)lua_tointeger(state, 2));
		lua_pushinteger(state, (lua_Integer)(hdc));
		return 1;
	}

	static int32_t _cdecl _lua_fillrect(
		struct lua_State* const state
	)
	{
		RECT rect{};
		rect.bottom = (LONG)lua_tointeger(state, 3);
		rect.left = (LONG)lua_tointeger(state, 4);
		rect.right = (LONG)lua_tointeger(state, 5);
		rect.top = (LONG)lua_tointeger(state, 6);
		lua_pushinteger(state, (lua_Integer)(FillRect((HDC)lua_tointeger(state, 1), &rect, (HBRUSH)lua_tointeger(state, 2))));
		return 1;
	}

	static int32_t _cdecl _lua_endpaint(
		struct lua_State* const state
	)
	{
		const BOOL result = EndPaint((HWND)lua_tointeger(state, 1), (PAINTSTRUCT*)lua_tointeger(state, 2));
		lua_pushboolean(state,result);
		return 1;
	}

	static int32_t _cdecl _lua_allocpaintstruct(
		struct lua_State* const state
	)
	{
		PAINTSTRUCT* const ps = new PAINTSTRUCT();
		lua_pushinteger(state, (lua_Integer)(ps));
		return 1;
	}

	static int32_t _cdecl _lua_freepaintstruct(
		struct lua_State* const state
	)
	{
		PAINTSTRUCT* const ps = (PAINTSTRUCT * const)lua_tointeger(state, 1);
		if (ps) delete(ps);
		lua_pushinteger(state, (lua_Integer)(0));
		return 1;
	}

	static int32_t _cdecl _lua_setpixel(
		struct lua_State* const state
	)
	{

		lua_pushinteger(state,
			(lua_Integer)SetPixel(
			(HDC)lua_tointeger(state,1),
				(int)lua_tointeger(state,2),
				(int)lua_tointeger(state,3),
				(COLORREF)lua_tointeger(state,4)
			));
		return 1;
	}

	static int32_t _cdecl _lua_createcompatibledc(
		struct lua_State* const state
	)
	{
		lua_pushinteger(state,(lua_Integer)(CreateCompatibleDC((HDC)(lua_tointeger(state, 1)))));
		return 1;
	}

	static int32_t _cdecl _lua_deletedc(
		struct lua_State* const state
	)
	{
		lua_pushboolean(state, (lua_Integer)(DeleteDC((HDC)(lua_tointeger(state, 1)))));
		return 1;
	}

	static int32_t _cdecl _lua_bitblt(
		struct lua_State* const state
	)
	{
		lua_pushboolean(state,
			BitBlt(
			(HDC)lua_tointeger(state,1),
				(int)lua_tointeger(state, 2),
				(int)lua_tointeger(state, 3),
				(int)lua_tointeger(state, 4),
				(int)lua_tointeger(state, 5),
				(HDC)lua_tointeger(state, 6),
				(int)lua_tointeger(state, 7),
				(int)lua_tointeger(state, 8),
				(DWORD)lua_tointeger(state, 9)));
		return 1;
	}

	

	static BOOL CALLBACK _callback_enumproc(HWND hwnd, LPARAM lp)
	{
		{
			struct cfpair* const cp = (cfpair * const)lp;

			lua_getglobal(cp->state, cp->foo.c_str());
			lua_pushinteger(cp->state, (lua_Integer)hwnd);
			lua_call(cp->state, 1, 0);
		}
		//lua_pop(cp->state, 1);

		return true;
	}

	static int32_t _cdecl _lua_releasedc(
		struct lua_State* const state
	)
	{
	lua_pushinteger(state,ReleaseDC((HWND)lua_tointeger(state,1), (HDC)lua_tointeger(state,2)));
	return 0;
	}

	static int32_t _cdecl _lua_messagebox(
		struct lua_State* const state
	)
	{
		const int32_t result = MessageBoxA((HWND)(lua_tointeger(state, 1)), lua_tostring(state, 2), lua_tostring(state, 3), (UINT)(lua_tointeger(state, 4)));
		lua_pushinteger(state, (lua_Integer)(result));
		return 1;
	}
	
	static int32_t _cdecl _lua_getdc(
		struct lua_State* const state
	)
	{
		lua_pushinteger(state,(lua_Integer)GetDC((HWND)lua_tointeger(state, 1)));
		return 1;
	}

	static int32_t _cdecl _lua_enumwindows(
		struct lua_State* const state
	)
	{

		struct cfpair* const  cp = new struct cfpair();
		cp->state = state;
		cp->foo = lua_tostring(state, 1);

		lua_pushboolean(state, EnumWindows(_callback_enumproc,(LPARAM)(cp)));

		if(cp)
		delete cp;

		return 1;
	}

	static int32_t _cdecl _lua_findwindow(
		struct lua_State* const state
	)
	{
	lua_pushinteger(state,(lua_Integer)::FindWindowExA(0,0, lua_tostring(state, 1),lua_tostring(state,2)));
	return 1;
	}

	static int32_t _cdecl _lua_setcursorposition(
		struct lua_State* const state
	)
	{
		lua_pushboolean(state,SetCursorPos((int)lua_tointeger(state, 1), (int)lua_tointeger(state, 2)));
		return 1;
	}

	static int32_t _cdecl _lua_setwallpaper(
		struct lua_State* const state
	)
	{
		lua_pushboolean(state,SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0,(PVOID)lua_tostring(state,1), SPIF_UPDATEINIFILE));
		return 1;
	}

	static int32_t _cdecl _lua_hidemenu(
		struct lua_State* const state
	)
	{
	SetMenu((HWND)lua_tointeger(state,1),NULL);
	return 0;
	}

	static int32_t _cdecl _lua_registerevent(
		struct lua_State* const state
	)
	{

	const lua_Integer Event = lua_tointeger(state, 1);
	lua_events[(uint32_t)Event] = lua_tostring(state, 2);


	switch (Event)
	{
	case WM_PAINT:
		EventPaint = [](lua_State* state)
		{ 
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state,lua_events[WM_PAINT].c_str());
			lua_pcall(state, 0, 0,0);
			return 0; 
		};
		break;
	case WM_DESTROY:
		EventDestroy = [](lua_State* state)
		{
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state, lua_events[WM_DESTROY].c_str());
			lua_pcall(state, 0, 0,0);
			return 0;
		};
		break;
	case WM_TIMER:
		EventTimer = [](lua_State* state)
		{
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state, lua_events[WM_TIMER].c_str());
			lua_pcall(state, 0, 0,0);
			return 0;
		};
		break;
	case WM_MOUSEHOVER:
		EventHover = [](lua_State* state)
		{
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state, lua_events[WM_MOUSEHOVER].c_str());
			lua_pcall(state, 0, 0, 0);
			return 0;
		};
		break;
	case WM_CLOSE:
		EventClose = [](lua_State* state)
		{
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state, lua_events[WM_CLOSE].c_str());
			lua_pcall(state, 0, 0, 0);
			return 0;
		};
		break;
	case WM_QUIT:
		EventQuit = [](lua_State* state)
		{
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state, lua_events[WM_QUIT].c_str());
			lua_pcall(state, 0, 0, 0);
			return 0;
		};
	case WM_CREATE:
		EventQuit = [](lua_State* state)
		{
			lua_pcall(state, 0, 0, 0);
			lua_getglobal(state, lua_events[WM_CREATE].c_str());
			lua_pcall(state, 0, 0, 0);
			return 0;
		};
		break;
	}
	
	return 0;
}

	constexpr long FOO_COUNT = 55;
	
	const char* sckeys[FOO_COUNT] = {
		"AllocPaintStruct",		   
		"BeginPaint",			   
		"BitBlt",				   
		"BlockInput",			   
		"ClientToScreen",		   
		"CursorPosition",		   
		"CreateCompatibleDC",	   
		"CreateWindow",			   
		"DeleteDC",				   
		"DestroyWindow",		   
		"DispatchMessage",		   
		"EndPaint",				   
		"EnumWindows",			   
		"Find",		
		"FillRect",
		"FreePaintStruct",		   
		"GetDC",
		"GetClassName",			   
		"GetDesktop",			   
		"GetForeground",		   
		"GetMessage",			   
		"GetName",				   
		"GetProcessId",	
		"GetClientRect",
		"GetRect",				   
		"GetScreenMetrics",		   
		"GetWindowHandle",		   
		"HideMenu",				   
		"InvalidateRect",		   
		"KeyDown",				   
		"KeyPressed",			   
		"KillTimer",			   
		"MessageBox",			   
		"PeekMessage",			   
		"PostMessage",			   
		"PostQuitMessage",		   
		"Proc",					   
		"RegisterEvent",		   
		"RegisterHotkey",		   
		"ReleaseDC",			   
		"RetrieveMessage",		   
		"SendMessage",			   
		"SetActive",		
		"SetCursorPosition",
		"SetFocus",				   
		"SetForeground",		   
		"SetPixel",				   
		"SetTimer",				   
		"SetTitle",				
		"SetWallpaper",
		"TranslateMessage",		   
		"Show",					   
		"Update",				   
		"ValidateRect",			   
		"WindowFromPoint"		   
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		_lua_allocpaintstruct,
		_lua_beginpaint,
		_lua_bitblt,
		_lua_blockinput,
		_lua_clienttoscreen,
		_lua_cursorposition,
		_lua_createcompatibledc,
		_lua_createwindow,
		_lua_deletedc,
		_lua_destroywindow,
		_lua_dispatchmessage,
		_lua_endpaint,
		_lua_enumwindows,
		_lua_findwindow,
		_lua_fillrect,
		_lua_freepaintstruct,
		_lua_getdc,
		_lua_getclassname,
		_lua_getdesktop,
		_lua_getforegroundwindow,
		_lua_getmessage,
		_lua_getname,
		_lua_getprocessid,
		_lua_getclientrect,
		_lua_getrect,
		_lua_getscreenmetrics,
		_lua_getwindowhandle,
		_lua_hidemenu,
		_lua_invalidaterect,
		_lua_keydown,
		_lua_keypressed,
		_lua_killtimer,
		_lua_messagebox,
		_lua_peekmessage,
		_lua_postmessage,
		_lua_postquitmessage,
		_lua_proc,
		_lua_registerevent,
		_lua_registerhotkey,
		_lua_releasedc,
		_lua_retrievemessage,
		_lua_sendmessage,
		_lua_setactive,
		_lua_setcursorposition,
		_lua_setfocus,
		_lua_setforeground,
		_lua_setpixel,
		_lua_settimer,
		_lua_settitle,
		_lua_setwallpaper,
		_lua_translatemessage,
		_lua_show,
		_lua_update,
		_lua_validaterect,
		_lua_windowfrompoint
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

