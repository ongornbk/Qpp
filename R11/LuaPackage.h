#pragma once
#include <map>
#include <string>

#include "LuaH.h"
#include "Settings.h"



typedef long(__stdcall * package_start_close_count_function)(void* ptrs);
typedef cpair(__stdcall * package_get_foo_function)(const long index);

class LuaPackage
{

	std::string m_name;
	std::string m_realName;
	std::string m_path;
	std::map<std::string, lua_CFunction> m_functions;

	HMODULE m_lib;

	lua_State* m_lua;

	package_start_close_count_function pckfoocount;
	package_start_close_count_function pckstart;
	package_start_close_count_function pckclose;
	package_get_foo_function pckgetfoo;
public:



	LuaPackage(lua_State* lua,std::string path,std::string name, std::string as)
	{
		m_path = path;
		m_lua = lua;
		m_name = as;
		m_realName = name;
	}

	LuaPackage(lua_State* lua, std::string path, std::string name)
	{
		m_path = path;
		m_name = name;
		m_lua = lua;
		m_realName = m_name;
	}

	~LuaPackage()
	{
		m_functions.clear();

		const long result = pckclose(0);
		if (result)
		{
			MessageBoxA(NULL, (m_realName + ".pck " + "closing failed! code -> " + std::to_string(result)).c_str(), "Dll Error", MB_OK);
		}
	}

	void push(std::string name, lua_CFunction foo)
	{
		m_functions[name] = foo;
	}

	void initialize()
	{
		m_lib = LoadLibraryA((m_path + ".pck").c_str());
		if (!m_lib)
		{
			std::string alt_path = GetSetting("DefaultPackagesLocation").get_string();

			m_lib = LoadLibraryA((alt_path + "/" + m_realName + ".pck").c_str());
			if (!m_lib)
			{
				throw std::exception((m_realName + ".pck " + "not found!").c_str());
			}
			
		}

		pckfoocount = package_start_close_count_function(GetProcAddress(m_lib, "foo_count"));
		pckstart = package_start_close_count_function(GetProcAddress(m_lib, "start"));
		pckclose = package_start_close_count_function(GetProcAddress(m_lib, "close"));
		pckgetfoo = package_get_foo_function(GetProcAddress(m_lib, "get_foo"));

		{
			long result = pckstart((void*)GetSetting("DefaultLibrariesLocation").c_str());
			if (result)
			{
				throw std::exception((m_realName + ".pck " + "starting failed! code -> " + std::to_string(result)).c_str());
			}
		}

		long foo_count = pckfoocount(0);
		
		for (long i = 0; i < foo_count; i++)
		{
			cpair cp = pckgetfoo(i);
			m_functions[cp.key] = cp.value;
		}

		long top = lua_gettop(m_lua);
		lua_newtable(m_lua);

		for (auto&& ele : m_functions)
		{
			lua_pushstring(m_lua, ele.first.c_str());   /* Push the table index */
			lua_pushcfunction(m_lua,ele.second); /* Push the cell value */
			lua_rawset(m_lua, -3);
		}

		lua_setglobal(m_lua,m_name.c_str());
	}
};