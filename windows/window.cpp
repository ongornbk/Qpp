#include "stdafx.h"
#include "Window.h"

lua_State* Window::m_state = nullptr;

bool _stdcall HandleEvent(lua_CFunction const foo)
{
	if (foo == nullptr)
		return true;
	foo(Window::m_state);
	return false;
}

lua_CFunction EventTimer = nullptr;
lua_CFunction EventDestroy = nullptr;
lua_CFunction EventPaint = nullptr;
lua_CFunction EventHover = nullptr;
lua_CFunction EventClose = nullptr;
lua_CFunction EventQuit = nullptr;


LRESULT __stdcall WindowProcedure(HWND window, uint32_t msg, WPARAM wp, LPARAM lp)

{
	switch (msg)
	
	{
	case WM_TIMER:
		if (HandleEvent(EventTimer))
			return DefWindowProc(window, msg, wp, lp);
		return 0;
	case WM_PAINT:
		if (HandleEvent(EventPaint))
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
	default:
	return DefWindowProc(window, msg, wp, lp);
	}

}

Window::Window(lua_State* state)
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