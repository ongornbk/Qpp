#include "pch.h"
#include "Windows.h"
#include "LuaManager.h"

struct Window
{
	Window()
	{
		
	}

	// Global Variables:
	HINSTANCE hInst;                               
	WCHAR szTitle[100];                 
	WCHAR szWindowClass[100];           

	ATOM                MyRegisterClass(HINSTANCE hInstance);
	BOOL                InitInstance(HINSTANCE, int);
	LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
};

namespace
{
	static lua_State* m_lua = nullptr;
	static std::unique_ptr<Window> m_window = nullptr;
	static HWND m_pickedWindow = NULL;
}

static int32_t _cdecl __PickForegroundWindow(lua_State* state)
{
	m_pickedWindow = GetForegroundWindow();
	return 0;
}

static int32_t _cdecl __ShowWindow(lua_State* state)
{
	ShowWindow(m_pickedWindow, lua_toboolean(state, 1));
	return 0;
}

static int32_t _cdecl __GetKeyState(lua_State* state)
{
	bool st = GetKeyState(lua_tointeger(state,1) & 0x100);
	lua_pushboolean(state, st);
	return 1;
}



static int32_t _cdecl __SendMessage(lua_State* state)
{
	SendMessage(m_pickedWindow, lua_tointeger(state, 1), lua_tointeger(state, 2), MAKELPARAM(lua_tointeger(state, 3), lua_tointeger(state, 4)));
	return 0;
}

static int32_t _cdecl CursorPosition(lua_State* state)
{
	POINT p;
	GetCursorPos(&p);
	p.y -= 22;
	lua_pushinteger(state, p.x);
	lua_pushinteger(state, p.y);
	return 2;
}

void _stdcall WindowsPackageInitializer()
{
	m_window = std::make_unique<Window>();
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "PickForegroundWindow", __PickForegroundWindow);
	lua_register(m_lua, "ShowWindow", __ShowWindow);
	lua_register(m_lua, "GetKeyState", __GetKeyState);
	lua_register(m_lua, "SendMessage", __SendMessage);
	lua_register(m_lua, "GetCursorPosition", CursorPosition);
}