#include "pch.h"
#include "Windows.h"
#include "LuaManager.h"

namespace
{
	static lua_State* m_lua = nullptr;
}

static int32_t _cdecl Mkdir(lua_State* state)
{
	lua_pushboolean(state, CreateDirectoryA(lua_tostring(state,1), NULL));
	return 1;
}

static int32_t _cdecl __GetEnv(lua_State* state)
{
#pragma warning(disable : 4996)
	lua_pushstring(state,getenv(lua_tostring(state,1)));
	return 1;
}

void _stdcall FilesystemPackageInitializer()
{

	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "Mkdir", Mkdir);
	lua_register(m_lua, "__GetEnv", __GetEnv);
}