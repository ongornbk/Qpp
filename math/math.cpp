// math.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "math.h"

extern "C"
{
	long start(const long arg)
	{

		return 0;
	}

	long close(const long arg)
	{

		return 0;
	}

	static int32_t _cdecl _lua_random(lua_State* state)
	{
		const int32_t min = (int32_t)lua_tointeger(state, 1);
		const int32_t max = (int32_t)lua_tointeger(state, 2);
		const int32_t ret = rand() % (max - min + 1) + min;
		lua_pushinteger(state, ret);
		return 1;
	}

	static int32_t _cdecl _lua_seed(lua_State* state)
	{
		srand((unsigned int)lua_tointeger(state, 1));
		return 0;
	}

	static int32_t _cdecl _lua_length(lua_State* state)
	{
		int32_t l{};
		int32_t i = (int32_t)lua_tointeger(state, 1);
		for (; i; i /= 10) l++;
		lua_pushinteger(state, l == 0 ? 1 : l);
		return 1;
	}

	static int32_t _cdecl _lua_tocolor(lua_State* state)
	{
		COLOR rgb;
		rgb.r = (uint8_t)lua_tointeger(state, 1);
		rgb.g = (uint8_t)lua_tointeger(state, 2);
		rgb.b = (uint8_t)lua_tointeger(state, 3);
		lua_pushinteger(state, rgb.color);
		return 1;
	}

	static int32_t _cdecl _lua_getcolor(lua_State* state)
	{
		COLOR rgb;
		rgb.color = (unsigned int)lua_tointeger(state, 1);
		lua_pushinteger(state, rgb.r);
		lua_pushinteger(state, rgb.g);
		lua_pushinteger(state, rgb.b);
		return 3;
	}

	static int32_t _cdecl _lua_firstday(lua_State* state)
	{
		int32_t year = (int32_t)lua_tointeger(state, 1);
		int32_t month = (int32_t)lua_tointeger(state, 2);
		int32_t day = 1;
		int32_t y = year - (14 - month) / 12;
		int32_t m = month + 12 * ((14 - month) / 12) - 2;
		int32_t r = (day + y + y / 4 - y / 100 + y / 400 + (31 * m / 12)) % 7;
		lua_pushinteger(state, r);
		return 1;
	}

	static int32_t _cdecl _lua_isleapyear(lua_State* state)
	{
		int32_t year = (int32_t)lua_tointeger(state, 1);

		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
			lua_pushboolean(state, true);
		else
			lua_pushboolean(state, false);
		return 1;
	}

	constexpr long FOO_COUNT = 7;
	
	const char* sckeys[FOO_COUNT] = {
	"FirstDay",
	"GetColor",
	"IsLeapYear",
	"Length",
	"Random",
	"Seed",
	"ToColor"
	};
	const lua_CFunction scfooes[FOO_COUNT] = {
	_lua_firstday,
	_lua_getcolor,
	_lua_isleapyear,
	_lua_length,
	_lua_random,
	_lua_seed,
	_lua_tocolor
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

