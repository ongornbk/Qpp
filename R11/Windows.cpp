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

LRESULT __stdcall WindowProcedure(HWND window, uint32_t msg, WPARAM wp, LPARAM lp)

{

	switch (msg)

	{

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
		break;
	default:
		return DefWindowProc(window, msg, wp, lp);

	}

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

	void Initialize()
	{
		const wchar_t* myclass = L"myclass";

		WNDCLASSEX wndclass;
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_DBLCLKS;
		wndclass.lpfnWndProc = WindowProcedure;
		wndclass.cbClsExtra = NULL;
		wndclass.cbWndExtra = NULL;
		wndclass.hInstance = GetModuleHandle(NULL);
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = myclass;
		wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (RegisterClassEx(&wndclass))
		{
			m_pickedWindow = CreateWindowEx(0, myclass, L"Window",WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
			if (m_pickedWindow)
			{
				ShowWindow(m_pickedWindow, SW_SHOWDEFAULT);
				MSG msg;
				while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
			}

		}
	}


};



static int32_t _cdecl __PickForegroundWindow(lua_State* state)
{
	m_pickedWindow = GetForegroundWindow();
	return 0;
}

static int32_t _cdecl __PickConsoleWindow(lua_State* state)
{
	m_pickedWindow = GetConsoleWindow();
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
HANDLE m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)lua_tointeger(state,1));
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

static int32_t _cdecl __CreateWindow(lua_State* state)
{
	m_window->Initialize();
	return 0;
}

void _stdcall WindowsPackageInitializer()
{
	m_window = std::make_unique<Window>();
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "CreateWindow", __CreateWindow);
	lua_register(m_lua, "PickForegroundWindow", __PickForegroundWindow); 
	lua_register(m_lua, "PickDesktopWindow", __PickDesktopWindow);
	lua_register(m_lua, "PickConsoleWindow", __PickConsoleWindow);
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