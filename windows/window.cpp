#include "stdafx.h"
#include "Window.h"

#include <stack>

struct lua_State* Window::m_state = nullptr;

namespace
{
	static std::stack<lua_Integer> m_ownstack;
}

bool _stdcall HandleEvent(lua_CFunction const foo)
{
	if (foo == nullptr)
		return true;
	foo(Window::m_state);
	return false;
}

bool _stdcall HandleTimer(lua_CFunction const foo,const lua_Integer id) // Temp
{
	if (foo == nullptr)
		return true;
	m_ownstack.push(id);
	foo(Window::m_state);
	return false;
}

bool _stdcall HandleKeyDown(lua_CFunction const foo, const lua_Integer id) //Temp
{
	if (foo == nullptr)
		return true;
	m_ownstack.push(id);
	foo(Window::m_state);
	return false;
}

lua_CFunction EventTimer = nullptr;
lua_CFunction EventDestroy = nullptr;
lua_CFunction EventPaint = nullptr;
lua_CFunction EventHover = nullptr;
lua_CFunction EventClose = nullptr;
lua_CFunction EventQuit = nullptr;
lua_CFunction EventCreate = nullptr;
lua_CFunction EventKeyDown = nullptr;

LRESULT __stdcall WindowProcedure(HWND window, uint32_t msg, WPARAM wp, LPARAM lp)

{
	switch (msg)
	
	{
	case WM_PAINT:
		if (HandleEvent(EventPaint))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_TIMER:
		if (HandleTimer(EventTimer,(lua_Integer)wp))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_KEYDOWN:
		if (HandleKeyDown(EventKeyDown, (lua_Integer)wp))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_MOUSEHOVER:
		if(HandleEvent(EventHover))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_DESTROY:
		if (HandleEvent(EventDestroy))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_CLOSE:
		if (HandleEvent(EventClose))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_QUIT:
		if (HandleEvent(EventQuit))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_CREATE:
		if (HandleEvent(EventCreate))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	default:
	return DefWindowProc(window, msg, wp, lp);
	}

}

Window::Window(
	struct lua_State* const state
)
{
	m_state = state;
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
		window = CreateWindowEx(0, myclass, L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
	}
}

Window::~Window()
{
	if (window)
	{
		DestroyWindow(window);
		//CloseHandle(window);
		window = NULL;
	}
}

std::stack<lua_Integer>& Window::GetStack()
{
	return m_ownstack;
}
