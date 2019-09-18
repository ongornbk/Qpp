// conio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "conio.h"
#include <conio.h>

std::string getline();

extern "C"
{

	static HANDLE HandleOut;
	static HANDLE HandleIn;
	static HWND   ConsoleWindow;
	static COORD  ConsoleCursorPosition;
	static HDC    ConsoleHDC;

	COORD GetConsoleCursorPosition(HANDLE const hConsoleOutput)
	{
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
		{
			return cbsi.dwCursorPosition;
		}
		else
		{
			COORD invalid = { 0, 0 };
			return invalid;
		}
	}

	long start(const long arg)
	{
		if (!AllocConsole())return 1;

#pragma warning(disable : 4996)
		if (!freopen("CONOUT$", "w", stdout))return 2;

		HandleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!HandleOut) return 3;

		HandleIn = GetStdHandle(STD_INPUT_HANDLE);
		if (!HandleIn) return 4;

		ConsoleWindow = GetConsoleWindow();
		if (!ConsoleWindow) return 5;

		ConsoleHDC = GetDC(ConsoleWindow);
		if (!ConsoleHDC) return 6;

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

	static int32_t _cdecl _lua_endline(lua_State* state)
	{
		printf_s("\n");
		return 0;
	}

	static int32_t _cdecl _lua_getline(lua_State* state)
	{
		lua_pushstring(state,getline().c_str());
		return 1;
	}

	static int32_t _cdecl _lua_getcursorposition(lua_State* state)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int32_t x, y;
		if (GetConsoleScreenBufferInfo(HandleOut, &csbi)) {
			x = csbi.dwCursorPosition.X;
			y = csbi.dwCursorPosition.Y;
		}
		lua_pushinteger(state, x);
		lua_pushinteger(state, y);
		return 2;
	}

	static int32_t _cdecl _lua_gotox(lua_State* state)
	{
		ConsoleCursorPosition = GetConsoleCursorPosition(HandleOut);
		ConsoleCursorPosition.X = (SHORT)lua_tointeger(state, 1);
		SetConsoleCursorPosition(HandleOut, ConsoleCursorPosition);
		return 0;
	}

	static int32_t _cdecl _lua_gotoy(lua_State* state)
	{
		ConsoleCursorPosition = GetConsoleCursorPosition(HandleOut);
		ConsoleCursorPosition.Y = (SHORT)lua_tointeger(state, 1);
		SetConsoleCursorPosition(HandleOut, ConsoleCursorPosition);
		return 0;
	}

	static int32_t _cdecl _lua_drawpixel(lua_State* state)
	{
		COORD _pos;
		_pos.X = (SHORT)lua_tointeger(state, 1);
		_pos.Y = (SHORT)lua_tointeger(state, 2);
		COLORREF color = (DWORD)lua_tointeger(state, 3);
		int32_t scale = (int32_t)lua_tointeger(state, 4);
		for (int i = 0; i < scale; i++)
			for (int j = 0; j <= scale; j++)
				SetPixel(ConsoleHDC, _pos.X + i, _pos.Y + j, color);
		return 0;
	}

	static int32_t _cdecl _lua_cls(lua_State* state)
	{
		ConsoleCursorPosition = { 0,0 };
		SetConsoleCursorPosition(HandleOut, ConsoleCursorPosition);

		BOOL bSuccess;
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD dwConSize;

		GetConsoleScreenBufferInfo(HandleOut, &csbi);
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		bSuccess = FillConsoleOutputCharacter(HandleOut,' ',
			dwConSize, ConsoleCursorPosition, &cCharsWritten);

		return 0;
	}

	static int32_t _cdecl _lua_fill(lua_State* state)
	{
		ConsoleCursorPosition = { 0,0 };
		SetConsoleCursorPosition(HandleOut, ConsoleCursorPosition);

		BOOL bSuccess;
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD dwConSize;

		GetConsoleScreenBufferInfo(HandleOut, &csbi);
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		bSuccess = FillConsoleOutputCharacterA(HandleOut, std::string(lua_tostring(state,1)).at(0),
			dwConSize, ConsoleCursorPosition, &cCharsWritten);

		return 0;
	}

	static int32_t _cdecl _lua_getch(lua_State* state)
	{
		lua_pushinteger(state, getch());
		return 1;
	}

	static int32_t _cdecl _lua_setcolor(lua_State* state)
	{
		SetConsoleTextAttribute(HandleOut,(unsigned short) lua_tointeger(state,1));
		return 0;
	}

	static int32_t _cdecl _lua_showcursor(lua_State* state)
	{
		CONSOLE_CURSOR_INFO     cursorInfo;
		GetConsoleCursorInfo(HandleOut, &cursorInfo);
		cursorInfo.bVisible = lua_toboolean(state,1);
		SetConsoleCursorInfo(HandleOut, &cursorInfo);
		return 0;
	}

	static int32_t _cdecl _lua_fwrite(lua_State* state)
	{
		fwrite(lua_tostring(state, 1),sizeof(char),lua_tointeger(state,2),stdout);
		return 0;
	}

	constexpr long FOO_COUNT = 18;

	const char* sckeys[FOO_COUNT] = {
		"Clear",
		"DrawPixel",
		"Endline",
		"Fill",
		"FWrite",
		"Getch",
		"GetCursorPosition",
		"Getline",
		"Gotox",
		"Gotoxy",
		"Gotoy",
		"Pause",
		"Print",
		"Printf",
		"Println",
		"SetColor",
		"SetTitle",
		"ShowCursor"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
		_lua_cls,
		_lua_drawpixel,
		_lua_endline,
		_lua_fill,
		_lua_fwrite,
		_lua_getch,
		_lua_getcursorposition,
		_lua_getline,
		_lua_gotox,
		_lua_gotoxy,
		_lua_gotoy,
		_lua_pause,
		_lua_print,
		_lua_printf,
		_lua_println,
		_lua_setcolor,
		_lua_setconsoletitle,
		_lua_showcursor
	};

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

std::string getline()
{
	long key{};
	char escape = true;
	std::string str;
	while (escape)
	{
		key = getch();
		switch (key)
		{
		case '\r':
		{
			escape = false;
			break;
		}
		case '\b':
		{
			ConsoleCursorPosition = GetConsoleCursorPosition(HandleOut);
			if (ConsoleCursorPosition.X != 0)
			{
				ConsoleCursorPosition.X--;
			}
			SetConsoleCursorPosition(HandleOut, ConsoleCursorPosition);
			printf_s("%c", ' ');
			SetConsoleCursorPosition(HandleOut, ConsoleCursorPosition);
			break;
		}

		default:
		{

			printf_s("%c", key);

			str += (char)key;
		}
		}
	}
	printf_s("\n");
	return str;
}