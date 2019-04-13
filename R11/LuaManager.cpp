#include "pch.h"
#include "LuaManager.h"
#include "Console.h"
#include "Windows.h"
#include "Urlmon.h"

namespace
{
	LuaManager* m_instance = nullptr;
}

static int32_t _cdecl ImportPackage(lua_State* state)
{

		std::string package;
		package = lua_tostring(LuaManager::GetInstance()->m_lua, 1);
		if (PackageMap::m_packages[package])
			PackageMap::m_packages[package]->Import();
		else
		{
			MessageBox(NULL, L"Error! Package not found!", L"Error!", NULL);
		}
	
	return 0;
}

static int32_t _cdecl GetArgument(lua_State* state)
{
	if (!m_instance)
	{
		lua_pushstring(state, "Lua not initialized!");
		return 1;
	}
	std::string argument;
	argument = m_instance->m_args.at(lua_tointeger(state, 1));
	lua_pushstring(state, argument.c_str());
	return 1;
}

static int32_t _cdecl GetNumberOfArguments(lua_State* state)
{
	if (!m_instance)
	{
		lua_pushstring(state, "Lua not initialized!");
		return 1;
	}
	std::string number = std::to_string(m_instance->m_numberOfArguments);
	lua_pushstring(state, number.c_str());
	return 1;
}

inline void LuaManager::CreatePackage(std::string name,void(*initializer)(void))
{
	Package* pck = new Package();
	pck->m_initializer = initializer;
	pck->m_name = name;
	m_packages[name] = pck;
}

LuaManager::LuaManager()
{
	m_instance = this;
	m_lua = nullptr;
	CreatePackage("conio", ConsolePackageInitializer);
	CreatePackage("math", MathPackageInitializer);
	CreatePackage("system", SystemPackageInitializer);
	CreatePackage("windows", WindowsPackageInitializer);
	CreatePackage("urlmon", UrlmonPackageInitializer);
	CreatePackage("filesystem", FilesystemPackageInitializer);
	CreatePackage("input", InputPackageInitializer);
	CreatePackage("network", NetworkPackageInitializer);
}

LuaManager::~LuaManager()
{
	lua_close(m_lua);
	m_lua = nullptr;
	for (auto&& obj : m_packages)
	{
		delete obj.second;
		obj.second = nullptr;
	}
	m_packages.clear();
}

bool _cdecl LuaManager::Initialize(const int argc, char* argv[])
{
	m_numberOfArguments = argc;
	m_path = argv[0];
	m_file = argv[1];

	for (int i = 0; i < (argc - 2); i++)
		m_args.push_back(argv[i + 2]);

	m_lua = luaL_newstate();
	OpenLibs();
	if (!m_lua)return false;
	bool result;
	lua_register(m_lua, "ImportPackage", ImportPackage);
	lua_register(m_lua, "GetArgument", GetArgument);
	lua_register(m_lua, "GetNumberOfArguments", GetNumberOfArguments);
	result = luaL_loadfile(m_lua, m_file.c_str());
	if (result)
	{
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(m_lua, 1));
		return false;
	}
	result = lua_pcall(m_lua, 0, LUA_MULTRET, 0);
	if (result != LUA_OK) {
		fprintf(stderr, "Lua pcall error: %s\n", lua_tostring(m_lua, 1));
		return false;
	}
	return true;
	
}

bool _cdecl LuaManager::Execute(std::string filename) noexcept
{
	return false;
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
