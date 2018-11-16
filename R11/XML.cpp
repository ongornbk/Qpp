#include "pch.h"
#include "XML.h"
#include "LuaManager.h"

namespace
{
	static lua_State* m_lua{};
}



void _stdcall XMLPackageInitializer()
{
	m_lua = LuaManager::GetInstance()->m_lua;
}