#include "pch.h"
#include "Math.h"
#include "LuaManager.h"
#include <math.h>
#include <chrono>

namespace
{
	static lua_State* m_lua = nullptr;
}

static int32_t _cdecl RandomInteger(lua_State* state)
{

	int32_t min = lua_tointeger(state, 1);
	int32_t max = lua_tointeger(state, 2);
	int32_t ret = rand() % (max - min + 1) + min;
	lua_pushinteger(state, ret);
	return 1;
}

static int32_t _cdecl IntegerLength(lua_State* state)
{
	int32_t i = lua_tointeger(state, 1);
		int32_t l = 0;
		for (; i; i /= 10) l++;
		l == 0 ? 1 : l;
		lua_pushinteger(state, l);
		return 1;
}

void _stdcall MathPackageInitializer()
{
	srand(time(NULL));
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "RandomInteger", RandomInteger);
	lua_register(m_lua, "IntegerLength", IntegerLength);
}