#include "pch.h"
#include "XML.h"
#include "LuaManager.h"
#include <rapidxml.hpp>

using namespace rapidxml;

namespace
{
	static lua_State* m_lua{};
}



void CALL_CONV XMLPackageInitializer()
{
	m_lua = LuaManager::GetInstance()->m_lua;
}