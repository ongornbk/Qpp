#include "pch.h"
#include "Windows.h"
#include "LuaManager.h"

struct Window;

namespace
{
	static lua_State* m_lua = nullptr;
	static std::unique_ptr<Window> m_window = nullptr;
	static HWND m_pickedWindow = NULL;
	static HANDLE m_handle = NULL;
}

struct Window
{
	Window()
	{
		
	}    

	~Window()
	{
		if (m_handle)
		{
			CloseHandle(m_handle);
			m_handle = NULL;
		}
	}


};



static int32_t _cdecl __PickForegroundWindow(lua_State* state)
{
	m_pickedWindow = GetForegroundWindow();
	return 0;
}

static int32_t _cdecl __PickDesktopWindow(lua_State* state)
{
	m_pickedWindow = GetDesktopWindow();
	return 0;
}

static int32_t _cdecl __ShowWindow(lua_State* state)
{
	ShowWindow(m_pickedWindow, lua_toboolean(state, 1));
	return 0;
}

static int32_t _cdecl __SetActiveWindow(lua_State* state)
{
	SetActiveWindow(m_pickedWindow);
	return 0;
}

static int32_t _cdecl __SetFocus(lua_State* state)
{
	SetFocus(m_pickedWindow);
	return 0;
}

static int32_t _cdecl __GetWindowRect(lua_State* state)
{
	RECT rect;
	GetWindowRect(m_pickedWindow,&rect);
	lua_pushinteger(state, rect.bottom);
	lua_pushinteger(state, rect.left);
	lua_pushinteger(state, rect.right);
	lua_pushinteger(state, rect.top);
	return 4;
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

static int32_t _cdecl Execute(lua_State* state)
{	
	ShellExecuteA(NULL, lua_tostring(state,1), lua_tostring(state, 2), NULL, NULL, SW_SHOWNORMAL);
	return 0;
}

static int32_t _cdecl GetWindowProcessID(lua_State* state)
{
DWORD PID;
::GetWindowThreadProcessId(m_pickedWindow, &PID);
lua_pushinteger(state, (int32_t)PID);
return 1;
}

static int32_t _cdecl PickCurrentProcess(lua_State* state)
{
	m_handle = GetCurrentProcess();
	return 0;
}

static int32_t _cdecl OpenProcess(lua_State* state)
{
HANDLE m_handle = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_TERMINATE | PROCESS_VM_OPERATION | PROCESS_VM_READ |PROCESS_VM_WRITE, FALSE, (DWORD)lua_tointeger(state,1));
if (m_handle)
{
	lua_pushboolean(state, true);
}
else
{
	lua_pushboolean(state, false);
}
return 1;
}

void _stdcall WindowsPackageInitializer()
{
	m_window = std::make_unique<Window>();
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "PickForegroundWindow", __PickForegroundWindow); 
	lua_register(m_lua, "PickDesktopWindow", __PickDesktopWindow);
	lua_register(m_lua, "GetWindowProcessID", GetWindowProcessID);
	lua_register(m_lua, "OpenProcess", OpenProcess);
	lua_register(m_lua, "PickCurrentProcess", PickCurrentProcess);
	lua_register(m_lua, "ShowWindow", __ShowWindow);
	lua_register(m_lua, "SetActiveWindow", __SetActiveWindow);
	lua_register(m_lua, "SetFocus", __SetFocus);
	lua_register(m_lua, "GetWindowRect", __GetWindowRect);
	lua_register(m_lua, "GetKeyState", __GetKeyState);
	lua_register(m_lua, "SendMessage", __SendMessage);
	lua_register(m_lua, "GetCursorPosition", CursorPosition);
	lua_register(m_lua, "Execute", Execute);
}