#include "pch.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include "LuaManager.h"

struct Console
{
	Console() 
	{
	console_window = GetConsoleWindow();
	console_dc = GetDC(console_window);
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	hin = GetStdHandle(STD_INPUT_HANDLE);

	DWORD events;
	CONSOLE_CURSOR_INFO cci;
	//cci.bVisible = true;
	//SetConsoleCursorInfo(hout, &cci);
	SetConsoleTextAttribute(hout, 10u);
	//SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	SetConsoleTitleA("main.lua");
	//SetConsoleDisplayMode(hout, CONSOLE_FULLSCREEN_MODE, 0);
	}

	HWND console_window;
	HDC console_dc;
	HANDLE hout;
	HANDLE hin;
	INPUT_RECORD inp;
	DWORD events;
};

namespace
{
	static lua_State* m_lua = nullptr;
	static std::unique_ptr<Console> m_console = nullptr;
}

static int32_t _cdecl Print(lua_State* state)
{
	std::string output;
	output = lua_tostring(state, 1);
	std::cout << output;
	return 0;
}

static int32_t _cdecl Println(lua_State* state)
{
	std::string output;
	output = lua_tostring(state, 1);
	std::cout << output<<std::endl;
	return 0;
}

static int32_t _cdecl Pause(lua_State* state)
{
	getchar();
	return 0;
}

static int32_t _cdecl GetInput(lua_State* state)
{
	std::string input;
	std::cin >> input;
	lua_pushstring(state, input.c_str());
	return 1;
}

static int32_t _cdecl __SetConsoleTitle(lua_State* state)
{
	std::string title = lua_tostring(state, 1);
	SetConsoleTitleA(title.c_str());
	return 0;
}

static int32_t _cdecl GotoXY(lua_State* state)
{
	COORD pos;
	pos.X = (SHORT)lua_tointeger(state, 1);
	pos.Y = (SHORT)lua_tointeger(state, 2);
	SetConsoleCursorPosition(m_console->hout, pos);
	return 0;
}

void DrawPixel(COORD p, COLORREF	color, int cscale) {

}

static int32_t _cdecl ConsoleDrawPixel(lua_State* state)
{
	COORD pos;
	pos.X = (SHORT)lua_tointeger(state, 1);
	pos.Y = (SHORT)lua_tointeger(state, 2);
	COLORREF color = lua_tointeger(state, 3);
	int32_t scale = lua_tointeger(state, 4);
	HDC dc = m_console->console_dc;
	for (int i = 0; i < scale; i++)
		for (int j = 0; j <= scale; j++)
			SetPixel(dc, pos.X + i, pos.Y + j, color);
	return 0;
}

void _stdcall ConsolePackageInitializer()
{
	m_console = std::make_unique<Console>();
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua,"Print", Print);
	lua_register(m_lua, "Println", Println);
	lua_register(m_lua, "Pause", Pause);
	lua_register(m_lua, "GetInput", GetInput);
	lua_register(m_lua, "SetConsoleTitle", __SetConsoleTitle);
	lua_register(m_lua, "Gotoxy", GotoXY);
	lua_register(m_lua, "ConsoleDrawPixel", ConsoleDrawPixel);
}
