#include "pch.h"
#include "LuaManager.h"

namespace
{
	LuaManager* m_instance = nullptr;
}


static int32_t _cdecl _lua_import(lua_State* state)
{
	std::string pckpath = lua_tostring(state, 1);
	std::string pckname;
	size_t off = pckpath.find_last_of("/\\");
	if (off != pckpath.npos)
		pckname = pckpath.substr(off+1);
	else
		pckname = pckpath;

	m_instance->m_pcks[pckname] = new LuaPackage(state,pckpath, pckname);

	try
	{
		m_instance->m_pcks[pckname]->initialize();
	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "Dll Error", MB_OK);
		return 0;
	}
	return 0;
}

static int32_t _cdecl _lua_import_as(lua_State* state)
{
	std::string pckpath = lua_tostring(state, 1);
	std::string pckas = lua_tostring(state, 2);

	std::string pckname;
	size_t off = pckpath.find_last_of("/\\");
	if (off != pckpath.npos)
		pckname = pckpath.substr(off+1);
	else
		pckname = pckpath;

	m_instance->m_pcks[pckname] = new LuaPackage(state,pckpath, pckname,pckas);
	try
	{
		m_instance->m_pcks[pckname]->initialize();
	}
	catch (std::exception exception)
	{
		MessageBoxA(NULL, exception.what(), "Dll Error", MB_OK);
		return 0;
	}
	return 0;
}

static int32_t _cdecl _lua_get_argument(lua_State* state)
{
	std::string argument;
	argument = m_instance->m_args.at(lua_tointeger(state, 1));
	lua_pushstring(LuaManager::GetInstance()->m_lua, argument.c_str());
	return 1;
}

static int32_t _cdecl _lua_get_argc(lua_State* state)
{
	std::string number = std::to_string(m_instance->m_numberOfArguments);
	lua_pushstring(LuaManager::GetInstance()->m_lua, number.c_str());
	return 1;
}

LuaManager::LuaManager()
{
	m_instance = this;
	m_lua = nullptr;

}

LuaManager::~LuaManager()
{
	lua_close(m_lua);
	m_lua = nullptr;

	for (auto&& obj : m_pcks)
	{
		if (obj.second)
		{
			delete obj.second;
			obj.second = nullptr;
		}
	}
	m_pcks.clear();
}

bool _cdecl LuaManager::Initialize(const int argc, char* argv[])
{
	m_numberOfArguments = argc;
	m_path = argv[0];
	m_file = argv[1];

	for (int i = 0; i < (argc); i++)
		m_args.push_back(argv[i]);

	m_lua = luaL_newstate();
	OpenLibs();
	if (!m_lua)return false;
	bool result;
	lua_register(m_lua, "import", _lua_import);
	lua_register(m_lua, "import_as", _lua_import_as);
	lua_register(m_lua, "get_argument", _lua_get_argument);
	lua_register(m_lua, "get_argc", _lua_get_argc);

	result = luaL_loadfile(m_lua, m_file.c_str());
	if (result)
	{
		MessageBoxA(NULL, ("Couldn't load file: " + m_file).c_str(), "Lua Error!", MB_OK);
		return false;
	}
	result = lua_pcall(m_lua, 0, LUA_MULTRET, 0);
	if (result != LUA_OK) {
		MessageBoxA(NULL, ("Lua pcall error: " + std::string(lua_tostring(m_lua, 1))).c_str(), "Lua Error!", MB_OK);
		return false;
	}
	return true;
	
}

std::string _cdecl LuaManager::GetPath() noexcept
{
	return m_path;
}

LuaManager * LuaManager::GetInstance() noexcept
{
	return m_instance;
}

void _cdecl LuaManager::OpenLibs() noexcept
{
	luaL_openlibs(m_lua);
}
