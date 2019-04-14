// conio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "conio.h"
#include <conio.h>

extern "C"
{

	static HANDLE HandleOut;
	static HANDLE HandleIn;

	long start(const long arg)
	{
		if (!AllocConsole())return 1;

#pragma warning(disable : 4996)
		if (!freopen("CONOUT$", "w", stdout))return 2;

		HandleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!HandleOut) return 3;

		HandleIn = GetStdHandle(STD_INPUT_HANDLE);
		if (!HandleIn) return 4;

		return 0;
	}

	long close(const long arg)
	{
		if (fclose(stdout)) return 1;

		if (!FreeConsole()) return 2;

		return 0;
	}

	

	static int32_t _cdecl _lua_pause(lua_State* state)
	{
		getch();
		return 0;
	}

	static int32_t _cdecl _lua_printf(lua_State* state)
	{
		printf_s(lua_tostring(state,1));
		return 0;
	}

	static int32_t _cdecl _lua_print(lua_State* state)
	{
		printf_s("%s",lua_tostring(state, 1));
		return 0;
	}

	static int32_t _cdecl _lua_println(lua_State* state)
	{
		printf_s("%s\n", lua_tostring(state, 1));
		return 0;
	}

	static int32_t _cdecl _lua_setconsoletitle(lua_State* state)
	{
		SetConsoleTitleA(lua_tostring(state, 1));
		return 0;
	}

	static int32_t _cdecl _lua_gotoxy(lua_State* state)
	{
		COORD pos;
		pos.X = (SHORT)lua_tointeger(state, 1);
		pos.Y = (SHORT)lua_tointeger(state, 2);
		SetConsoleCursorPosition(HandleOut, pos);
		return 0;
	}

	constexpr long FOO_COUNT = 6;

	const char* sckeys[FOO_COUNT] = {"Gotoxy", "Pause","Print","Printf","Println","SetConsoleTitle" };
	const lua_CFunction scfooes[FOO_COUNT] = { _lua_gotoxy,_lua_pause,_lua_print,_lua_printf,_lua_println,_lua_setconsoletitle };

	long foo_count(const long arg)
	{
		return FOO_COUNT;
	}

	cpair get_foo(const long index)
	{
		cpair cp;
		cp.key = sckeys[index];
		cp.value = scfooes[index];
		return cp;
	}
}
