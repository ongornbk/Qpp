#pragma once
#include <map>
#include <string>

#include "LuaH.h"

#include <iostream>

extern "C"
{
	struct cpair
	{
		const char* key;
		lua_CFunction value;
	};
}

typedef long(__stdcall * package_start_close_count_function)(const long arg);
typedef cpair(__stdcall * package_get_foo_function)(const long index);

class LuaPackage
{
	std::string m_name;
	std::string m_realName;
	std::map<std::string, lua_CFunction> m_functions;

	HMODULE m_lib;

	lua_State* m_lua;

	package_start_close_count_function pckfoocount;
	package_start_close_count_function pckstart;
	package_start_close_count_function pckclose;
	package_get_foo_function pckgetfoo;
public:



	LuaPackage(lua_State* lua,std::string name, std::string as)
	{
		m_lua = lua;
		m_realName = name;
		m_name = as;
	}

	LuaPackage(lua_State* lua, std::string name)
	{
		m_lua = lua;
		m_realName = name;
		m_name = name;
	}

	~LuaPackage()
	{
		m_functions.clear();

		long result = pckclose(0);
		if (result)
		{
			MessageBoxA(NULL, (m_realName + ".dll " + "closing failed! code -> " + std::to_string(result)).c_str(), "Dll Error", MB_OK);
		}
	}

	void push(std::string name, lua_CFunction foo)
	{
		m_functions[name] = foo;
	}

	void initialize()
	{
		m_lib = LoadLibraryA((m_realName + ".dll").c_str());
		if (!m_lib)
		{
			throw std::exception((m_realName + ".dll " + "not found!").c_str());
		}

		pckfoocount = package_start_close_count_function(GetProcAddress(m_lib, "foo_count"));
		pckstart = package_start_close_count_function(GetProcAddress(m_lib, "start"));
		pckclose = package_start_close_count_function(GetProcAddress(m_lib, "close"));
		pckgetfoo = package_get_foo_function(GetProcAddress(m_lib, "get_foo"));

		{
			long result = pckstart(0);
			if (result)
			{
				throw std::exception((m_realName + ".dll " + "starting failed! code -> " + std::to_string(result)).c_str());
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
			//lua_register(m_lua, ele.first.c_str(), ele.second);
			lua_pushstring(m_lua, ele.first.c_str());   /* Push the table index */
			lua_pushcfunction(m_lua,ele.second); /* Push the cell value */
			lua_rawset(m_lua, -3);
		}

		lua_setglobal(m_lua,m_name.c_str());

		//long top = lua_gettop(m_lua);
		//lua_createtable(m_lua,0,2);
		
		//for (auto it = m_functions.begin(); it != m_functions.end(); ++it) {

			//lua_pushcfunction(m_lua,it->second);
			//lua_setfield(m_lua, it->second.c_str());

			//setField(m_lua, "slot", i);
			//setField(m_lua, "kartaid", karta->getID());
		
			//lua_settop(m_lua, top);
		
	//	lua_settable(m_lua,top);
	//	lua_setglobal(m_lua, m_name.c_str());
	}
};