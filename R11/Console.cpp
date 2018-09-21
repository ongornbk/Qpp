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

	//DWORD events;
	//CONSOLE_CURSOR_INFO cci;
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

static int32_t _cdecl Endline(lua_State* state)
{
	printf("\n");
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
	std::getline(std::cin, input);
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

static int32_t _cdecl GetConsoleCursorPosition(lua_State* state)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int32_t x, y;
	if (GetConsoleScreenBufferInfo(m_console->hout, &csbi)) {
		x = csbi.dwCursorPosition.X;
		y = csbi.dwCursorPosition.Y;
	}
	lua_pushinteger(m_lua, x);
	lua_pushinteger(m_lua, y);
	return 2;
}

static int32_t _cdecl GotoX(lua_State* state)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD pos;
	pos.X = (SHORT)lua_tointeger(state, 1);
	if (GetConsoleScreenBufferInfo(m_console->hout, &csbi)) {
		pos.Y = csbi.dwCursorPosition.Y;
	}
	SetConsoleCursorPosition(m_console->hout, pos);
	return 0;
}

static int32_t _cdecl GotoY(lua_State* state)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD pos;
	pos.Y = (SHORT)lua_tointeger(state, 1);
	if (GetConsoleScreenBufferInfo(m_console->hout, &csbi)) {
		pos.X = csbi.dwCursorPosition.X;
	}
	SetConsoleCursorPosition(m_console->hout, pos);
	return 0;
}

static int32_t _cdecl ConsoleDrawPixel(lua_State* state)
{
	COORD pos;
	pos.X = (SHORT)lua_tointeger(state, 1);
	pos.Y = (SHORT)lua_tointeger(state, 2);
	COLORREF color = (DWORD)lua_tointeger(state, 3);
	int32_t scale = (int32_t)lua_tointeger(state, 4);
	HDC dc = m_console->console_dc;
	for (int i = 0; i < scale; i++)
		for (int j = 0; j <= scale; j++)
			SetPixel(dc, pos.X + i, pos.Y + j, color);
	return 0;
}

static int32_t _cdecl CleanConsole(lua_State* state)
{
	SetConsoleCursorPosition(m_console->hout, { 0,0 });
	system("cls");
	return 0;
}

static int32_t _cdecl __FreeConsole(lua_State* state)
{
	FreeConsole();
	return 0;
}

static int32_t _cdecl __AllocConsole(lua_State* state)
{
	AllocConsole();
	return 0;
}

void _stdcall ConsolePackageInitializer()
{
	
	m_console = std::make_unique<Console>();
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua,"Print", Print); 
	lua_register(m_lua, "Println", Println);
	lua_register(m_lua, "Endline", Endline);
	lua_register(m_lua, "Pause", Pause);
	lua_register(m_lua, "GetInput", GetInput);
	lua_register(m_lua, "SetConsoleTitle", __SetConsoleTitle);
	lua_register(m_lua, "Gotoxy", GotoXY);
	lua_register(m_lua, "Gotox", GotoX);
	lua_register(m_lua, "Gotoy", GotoY);
	lua_register(m_lua, "CleanConsole", CleanConsole);
	lua_register(m_lua, "ConsoleDrawPixel", ConsoleDrawPixel);
	lua_register(m_lua, "GetConsoleCursorPosition", GetConsoleCursorPosition);
	lua_register(m_lua, "FreeConsole", __FreeConsole);
	lua_register(m_lua, "AllocConsole", __AllocConsole);
}
