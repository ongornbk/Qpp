#include "pch.h"
#include "Math.h"
#include "LuaManager.h"
#include <math.h>
#include <chrono>


struct COLOR
{
	union
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};
		uint32_t color;

	};

};

namespace
{
	static lua_State* m_lua = nullptr;
}

static int32_t _cdecl RandomInteger(lua_State* state)
{

	int32_t min = (int32_t)lua_tointeger(state, 1);
	int32_t max = (int32_t)lua_tointeger(state, 2);
	int32_t ret = rand() % (max - min + 1) + min;
	lua_pushinteger(state, ret);
	return 1;
}

static int32_t _cdecl IntegerLength(lua_State* state)
{
	int32_t i = (int32_t)lua_tointeger(state, 1);
		int32_t l = 0;
		for (; i; i /= 10) l++;
		l == 0 ? 1 : l;
		lua_pushinteger(state, l);
		return 1;
}

static int32_t _cdecl ToColor(lua_State* state)
{
	COLOR rgb;
	rgb.r = (uint8_t)lua_tointeger(state, 1);
	rgb.g = (uint8_t)lua_tointeger(state, 2);
	rgb.b = (uint8_t)lua_tointeger(state, 3);
	lua_pushinteger(state, rgb.color);
	return 1;
}

static int32_t _cdecl GetColor(lua_State* state)
{
	COLOR rgb;
	rgb.color = (unsigned int)lua_tointeger(state, 1);
	lua_pushinteger(state, rgb.r);
	lua_pushinteger(state, rgb.g);
	lua_pushinteger(state, rgb.b);
	return 3;
}


static int32_t _cdecl SetFirstDay(lua_State* state)
{
	int32_t year = lua_tointeger(state, 1);
	int32_t month = lua_tointeger(state, 2);
	int32_t day = 1;
	int32_t y = year - (14 - month) / 12;
	int32_t m = month + 12 * ((14 - month) / 12) - 2;
	int32_t r = (day + y + y / 4 - y / 100 + y / 400 + (31 * m / 12)) % 7;
	lua_pushinteger(state, r);
	return 1;
}

static int32_t _cdecl IsLeapYear(lua_State* state)
{
	int32_t year = lua_tointeger(state, 1);
	
	if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
	lua_pushboolean(state, true);
	else
	lua_pushboolean(state, false);
	return 1;
}

static int32_t _cdecl VarLength(lua_State* state)
{
	int32_t l{};
	int32_t i = lua_tointeger(state, 1);
	for (; i; i /= 10) l++;
	lua_pushinteger(state, l == 0 ? 1 : l);
	return 1;
}

void CALL_CONV MathPackageInitializer()
{
	srand((uint32_t)time(NULL));
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "RandomInteger", RandomInteger);
	lua_register(m_lua, "IntegerLength", IntegerLength);
	lua_register(m_lua, "ToColor", ToColor);
	lua_register(m_lua, "GetColor", GetColor);
	lua_register(m_lua, "SetFirstDay", SetFirstDay);
	lua_register(m_lua, "IsLeapYear", IsLeapYear);
	lua_register(m_lua, "VarLength", VarLength);
}