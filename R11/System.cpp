#include "pch.h"
#include "System.h"
#include "LuaManager.h"
#include <stdio.h>
#include <cstdio>
#include <thread>

using namespace std::chrono;

namespace
{
	static lua_State* m_lua = nullptr;
}

static int32_t _cdecl __Sleep(lua_State* state)
{
	uint64_t time = (uint64_t)lua_tointeger(state, 1);
	std::this_thread::sleep_for(1ns*time);
	return 0;
}

static int32_t _cdecl __Shutdown(lua_State* state)
{
	system("shutdown -s");
	return 0;
}

static int32_t _cdecl __System(lua_State* state)
{
	std::string str = lua_tostring(state, 1);
	system(str.c_str());
	return 0;
}

static int32_t _cdecl __GetEnv(lua_State* state)
{
#pragma warning(disable : 4996)
	std::string str = getenv(lua_tostring(state, 1));
	lua_pushstring(state, str.c_str());
	return 1;
}

static int32_t _cdecl __SetAttributes(lua_State* state)
{
	std::string str = lua_tostring(state, 1);
	uint64_t attribute = lua_tointeger(state, 2);
	SetFileAttributesA(str.c_str(),attribute);
	return 0;
}





void _stdcall SystemPackageInitializer()
{
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "Sleep", __Sleep);
	lua_register(m_lua, "Shutdown", __Shutdown);
	lua_register(m_lua, "System", __System);
	lua_register(m_lua, "GetEnv", __GetEnv);
	lua_register(m_lua, "SetAttributes", __SetAttributes);
}