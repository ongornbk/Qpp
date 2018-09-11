#include "pch.h"
#include "System.h"
#include "LuaManager.h"
#include <stdio.h>

namespace
{
	static lua_State* m_lua = nullptr;
}

static int32_t _cdecl __Sleep(lua_State* state)
{
	int32_t time = lua_tointeger(state, 1);
	bool cnt = lua_toboolean(state, 2);
	if (cnt)
	{
		printf("\n");
		while (time > 0)
		{
			printf("%d seconds\r",time);
			Sleep(1000);
			time--;
		}
	}
	else
	{
		while (time > 0)
		{
			Sleep(1000);
			time--;
		}
	}

	return 0;
}



static int32_t _cdecl __Shutdown(lua_State* state)
{
	system("shutdown -s");
	return 0;
}

void _stdcall SystemPackageInitializer()
{
	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "Sleep", __Sleep);
	lua_register(m_lua, "Shutdown", __Shutdown);
}