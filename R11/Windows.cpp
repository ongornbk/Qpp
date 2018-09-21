#include "pch.h"
#include "Windows.h"
#include "LuaManager.h"
#include "WindowC.h"

struct Window;

namespace
{
	static lua_State* m_lua = nullptr;
	static std::unique_ptr<Window> m_window = nullptr;
	static HANDLE m_handle = NULL;
	static HINSTANCE m_hinstance = NULL;
	static HDC m_pickedDC = NULL;
	static std::unique_ptr<HBITMAP> m_bitmap = NULL;
	static MSG m_msg;
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

static int32_t _cdecl __SendMessage(lua_State* state)
{
	SendMessage(m_pickedWindow, lua_tointeger(state, 1), lua_tointeger(state, 2), MAKELPARAM(lua_tointeger(state, 3), lua_tointeger(state, 4)));
	return 0;
}

static int32_t _cdecl __SendBroadcastMessage(lua_State* state)
{
	SendMessage(HWND_BROADCAST, lua_tointeger(state, 1), lua_tointeger(state, 2), MAKELPARAM(lua_tointeger(state, 3), lua_tointeger(state, 4)));
	return 0;
}

static int32_t _cdecl __PostMessage(lua_State* state)
{
	PostMessage(m_pickedWindow, lua_tointeger(state, 1), lua_tointeger(state, 2), MAKELPARAM(lua_tointeger(state, 3), lua_tointeger(state, 4)));
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

static int32_t _cdecl __ClientToScreen(lua_State* state)
{
	POINT p;
	p.x = lua_tointeger(state, 1);
	p.y = lua_tointeger(state, 2);
	ClientToScreen(m_pickedWindow, &p);
	return 0;
}

static int32_t _cdecl __SetCursorPosition(lua_State* state)
{
	SetCursorPos(lua_tointeger(state, 1), lua_tointeger(state, 2));
	return 0;
}

static int32_t _cdecl __GetDC(lua_State* state)
{
	m_pickedDC = GetDC(m_pickedWindow);
	return 0;
}

static int32_t _cdecl __CreateCompatibleDC(lua_State* state)
{
	m_pickedDC = CreateCompatibleDC(0);
	return 0;
}

static int32_t _cdecl __CreateCompatibleBitmap(lua_State* state)
{
	m_bitmap = std::make_unique<HBITMAP>(CreateCompatibleBitmap(m_pickedDC, lua_tointeger(state, 1), lua_tointeger(state, 2)));
	return 0;
}

static int32_t _cdecl __SelectObject(lua_State* state)
{
	SelectObject(m_pickedDC, m_bitmap.get());
	return 0;
}

static int32_t _cdecl __BitBlt(lua_State* state)
{
	BitBlt(m_pickedDC, 0,0,lua_tointeger(state,1),lua_tointeger(state,2),GetDC(0),0,0,SRCCOPY);
	return 0;
}

static int32_t _cdecl __MaximizeWindow(lua_State* state)
{
	ShowWindow(m_pickedWindow, SW_SHOWMAXIMIZED);
	return 0;
}

static int32_t _cdecl __MinimizeWindow(lua_State* state)
{
	ShowWindow(m_pickedWindow, SW_SHOWMINIMIZED);
	return 0;
}

static int32_t _cdecl __UpdateWindow(lua_State* state)
{
	UpdateWindow(m_pickedWindow);
	return 0;
}

static int32_t _cdecl __SetWindowTitle(lua_State* state)
{
	SetWindowTextA(m_pickedWindow,lua_tostring(state,1));
	return 0;
}

static int32_t _cdecl __FindWindow(lua_State* state)
{
	m_pickedWindow = ::FindWindowExA(0,0, lua_tostring(state, 1),lua_tostring(state,2));
	if (m_pickedWindow)lua_pushboolean(state, true);
	else lua_pushboolean(state, false);
	return 1;
}

static int32_t _cdecl __GetWindowName(lua_State* state)
{
	char name[100];
	GetWindowTextA(m_pickedWindow, name, 100);
	lua_pushstring(state, name);
	return 1;
}

static int32_t _cdecl __GetClassName(lua_State* state)
{
	char name[100];
	GetClassNameA(m_pickedWindow, name, 100);
	lua_pushstring(state, name);
	return 1;
}

static int32_t _cdecl __RegisterHotKey(lua_State* state)
{
	RegisterHotKey(NULL, int32_t(lua_tointeger(state, 1)), MOD_NOREPEAT, uint32_t(lua_tointeger(state, 2)));
	return 1;
}

static int32_t _cdecl __GetMessage(lua_State* state)
{
	lua_pushboolean(state,GetMessage(&m_msg, NULL, NULL, NULL));
	return 1;
}

static int32_t _cdecl __PeekMessage(lua_State* state)
{
	lua_pushboolean(state, PeekMessage(&m_msg, NULL, NULL,NULL, 0x0001));
	return 1;
}

static int32_t _cdecl RetrieveMessage(lua_State* state)
{
	lua_pushinteger(state, (int32_t)m_msg.lParam);
	lua_pushinteger(state, (int32_t)m_msg.wParam);
	lua_pushinteger(state, (int32_t)m_msg.message);
	return 3;
}

int32_t _cdecl keyPressed(int key) noexcept
{
	return (GetKeyState(key) & 0x8000 != 0);
}

int32_t _cdecl keyDown(int key) noexcept
{
	return ((GetKeyState(key) & 0x100) != 0);
}

static int32_t _cdecl KeyPressed(lua_State* state)
{
	lua_pushboolean(state, keyPressed(lua_tointeger(state, 1)));
	return 1;
}

static int32_t _cdecl KeyDown(lua_State* state)
{
	lua_pushboolean(state, keyDown(lua_tointeger(state, 1)));
	return 1;
}

void _stdcall WindowsPackageInitializer()
{
	ZeroMemory(&m_msg, sizeof(MSG));
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
	lua_register(m_lua, "SendMessage", __SendMessage);
	lua_register(m_lua, "SendBroadcastMessage", __SendBroadcastMessage);
	lua_register(m_lua, "PostMessage", __PostMessage);
	lua_register(m_lua, "GetCursorPosition", CursorPosition);
	lua_register(m_lua, "ClientToScreen", __ClientToScreen);
	lua_register(m_lua, "SetCursorPosition", __SetCursorPosition);
	lua_register(m_lua, "UpdateWindow", __UpdateWindow);
	lua_register(m_lua, "GetDC", __GetDC);
	lua_register(m_lua, "CreateCompatibleDC", __CreateCompatibleDC);
	lua_register(m_lua, "CreateCompatibleBitmap", __CreateCompatibleBitmap);
	lua_register(m_lua, "SelectObject", __SelectObject);
	lua_register(m_lua, "BitBlt", __BitBlt);
	lua_register(m_lua, "MinimizeWindow", __MinimizeWindow);
	lua_register(m_lua, "MaximizeWindow", __MaximizeWindow);
	lua_register(m_lua, "Execute", Execute);
	lua_register(m_lua, "SetWindowTitle", __SetWindowTitle);
	lua_register(m_lua, "FindWindow", __FindWindow);
	lua_register(m_lua, "GetWindowName", __GetWindowName);
	lua_register(m_lua, "GetClassName", __GetClassName);
	lua_register(m_lua, "RegisterHotKey", __RegisterHotKey);
	lua_register(m_lua, "GetMessage", __GetMessage);
	lua_register(m_lua, "PeekMessage", __PeekMessage);
	lua_register(m_lua, "RetrieveMessage", RetrieveMessage);
	lua_register(m_lua, "KeyPressed", KeyPressed);
	lua_register(m_lua, "KeyDown", KeyDown);
}