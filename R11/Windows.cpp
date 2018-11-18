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

	case WM_PAINT:

		InvalidateRect(m_pickedWindow, 0, false);
		return 0;
		break;
	case WM_DESTROY:
		m_pickedWindow = NULL;
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
				//ShowWindow(m_pickedWindow, SW_SHOWDEFAULT);
				//MSG msg;
				//while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
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

static int32_t _cdecl __SendInput(lua_State* state)
{
	INPUT ip;
	ip.type = lua_tointeger(state, 1);
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = lua_tointeger(state, 3);
	ip.ki.dwFlags = lua_tointeger(state,2);
	SendInput(1, &ip, sizeof(INPUT));
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
	BitBlt(m_pickedDC, 0,0,lua_tointeger(state,1),lua_tointeger(state,2),GetDC(m_pickedWindow),0,0,SRCCOPY);
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

void __private__DrawLine(HDC hdc, float StartX, float StartY, float EndX, float EndY, COLORREF Pen)
{
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 2, Pen);
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL);
	a = LineTo(hdc, EndX, EndY);
	DeleteObject(SelectObject(hdc, hOPen));
}

void __private__DrawString(HDC hdc, HFONT font, int x, int y, COLORREF color, const  char* text)
{
	SetTextAlign(hdc, TA_CENTER | TA_NOUPDATECP);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color);
	SelectObject(hdc, font);
	TextOutA(hdc, x, y, text, strlen(text));
	DeleteObject(font);
}

static int32_t _cdecl __DrawLine(lua_State* state)
{
	__private__DrawLine(m_pickedDC, lua_tointeger(state, 1), lua_tointeger(state, 2), lua_tointeger(state, 3), lua_tointeger(state, 4), lua_tointeger(state, 5));
	return 0;
}

static int32_t _cdecl __BlockInput(lua_State* state)
{
	lua_pushboolean(state,BlockInput(lua_toboolean(state, 1)));
	return 1;
}

static int32_t _cdecl _DrawString(lua_State* state)
{
	HFONT font = CreateFontA(20, 20, 2, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0);
	__private__DrawString(m_pickedDC, font, lua_tointeger(state, 1), lua_tointeger(state, 2), lua_tointeger(state, 3), lua_tostring(state, 4));
	return 1;
}

static int32_t _cdecl _HideMenu(lua_State* state)
{
	SetMenu(m_pickedWindow,NULL);
	return 0;
}

static int32_t _cdecl GetScreenMetrics(lua_State* state)
{
	lua_pushinteger(state, GetSystemMetrics(SM_CXSCREEN));
	lua_pushinteger(state, GetSystemMetrics(SM_CYSCREEN));
	return 2;
}

static int32_t _cdecl _ReleaseDC(lua_State* state)
{
	ReleaseDC(m_pickedWindow, m_pickedDC);
	return 0;
}

static int32_t _cdecl _SetPixel(lua_State* state)
{



	COORD pos;
	pos.X = (SHORT)lua_tointeger(state, 1);
	pos.Y = (SHORT)lua_tointeger(state, 2);
	COLORREF color = (DWORD)lua_tointeger(state, 3);
	int32_t scale = (int32_t)lua_tointeger(state, 4);
	for (int i = 0; i < scale; i++)
		for (int j = 0; j <= scale; j++)
			SetPixel(m_pickedDC, pos.X + i, pos.Y + j, color);
	return 0;
}

static int32_t _cdecl _ValidateRect(lua_State* state)
{
	ValidateRect(m_pickedWindow, 0);
	return 0;
}

static int32_t _cdecl _InvalidateRect(lua_State* state)
{
	InvalidateRect(m_pickedWindow, 0,false);
	return 0;
}

static int32_t _cdecl _SetTimer(lua_State* state)
{
	SetTimer(m_pickedWindow, lua_tointeger(state, 2),lua_tointeger(state,1), 0);
	return 0;
}

static int32_t _cdecl _KillTimer(lua_State* state)
{
	KillTimer(m_pickedWindow, lua_tointeger(state, 1));
	return 0;
}

static int32_t _cdecl WindowProc(lua_State* state)
{
	WindowProcedure(m_pickedWindow, lua_tointeger(state, 1), lua_tointeger(state, 2), lua_tointeger(state, 3));
	return 0;
}

static int32_t _cdecl _DispatchMessage(lua_State* state)
{
	DispatchMessage(&m_msg);
	return 0;
}

static int32_t _cdecl _PostQuitMessage(lua_State* state)
{
	PostQuitMessage(0);
	return 0;
}

static int32_t _cdecl WindowExist(lua_State* state)
{
	lua_pushboolean(state, (bool)m_pickedWindow);
	return 0;
}



void CALL_CONV WindowsPackageInitializer()
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
	lua_register(m_lua, "SendInput", __SendInput);
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
	lua_register(m_lua, "BlockInput", __BlockInput);
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
	lua_register(m_lua, "DrawLine", __DrawLine);
	lua_register(m_lua, "DrawString", _DrawString);
	lua_register(m_lua, "HideMenu", _HideMenu);
	lua_register(m_lua, "GetScreenMetrics", GetScreenMetrics);
	lua_register(m_lua, "ReleaseDC", _ReleaseDC);
	lua_register(m_lua, "SetPixel", _SetPixel);
	lua_register(m_lua, "ValidateRect", _ValidateRect);
	lua_register(m_lua, "InvalidateRect", _InvalidateRect);
	lua_register(m_lua, "SetTimer", _SetTimer);
	lua_register(m_lua, "KillTimer", _KillTimer);
	lua_register(m_lua, "WindowProc", WindowProc);
	lua_register(m_lua, "DispatchMessage", _DispatchMessage);
	lua_register(m_lua, "PostQuitMessage", _PostQuitMessage);
	lua_register(m_lua, "WindowExist", WindowExist);
}