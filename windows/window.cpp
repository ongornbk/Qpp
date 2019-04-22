#include "stdafx.h"
#include "Window.h"

int32_t _stdcall doNothing(lua_State* state)
{
	return 0;
}

lua_CFunction EventTimer = doNothing;
lua_CFunction EventDestroy = doNothing;
lua_CFunction EventPaint = doNothing;
lua_CFunction EventHover = doNothing;


LRESULT __stdcall WindowProcedure(HWND window, uint32_t msg, WPARAM wp, LPARAM lp)

{
	
	switch (msg)
	
	{
	case WM_TIMER:
		//EventTimer(state);
		return 0;
	//case WM_PAINT:
		//if (EventPaint == doNothing)
			//return DefWindowProc(window, msg, wp, lp);
		//EventPaint(state);
		//return 0;
	case WM_MOUSEHOVER:
		//EventHover(state);
		return 0;
	case WM_DESTROY:
		//EventDestroy(state);
		return 0;
	default:
	return DefWindowProc(window, msg, wp, lp);
	}

}

Window::Window()
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