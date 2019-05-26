#include "pch.h"
#include "UnmanagedPackage.h"
#include "Settings.h"


	UnmanagedPackage::UnmanagedPackage(lua_State* lua, std::string path, std::string name, std::string as)
	{
		m_path = path;
		m_lua = lua;
		m_name = as;
		m_realName = name;
	}

	UnmanagedPackage::UnmanagedPackage(lua_State* lua, std::string path, std::string name)
	{
		m_path = path;
		m_name = name;
		m_lua = lua;
		m_realName = m_name;
	}

	UnmanagedPackage::~UnmanagedPackage()
	{
		m_functions.clear();

		const long result = pckclose(0);
		if (result)
		{
			MessageBoxA(NULL, (m_realName + ".pck " + "closing failed! code -> " + std::to_string(result)).c_str(), "Dll Error", MB_OK);
		}
	}

	void UnmanagedPackage::push(std::string name, cfoo foo)
	{
		m_functions[name] = (lua_CFunction)foo;
	}

	void UnmanagedPackage::initialize()
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
			long result = pckstart(0);
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
			lua_pushcfunction(m_lua, ele.second); /* Push the cell value */
			lua_rawset(m_lua, -3);
		}

		lua_setglobal(m_lua, m_name.c_str());
	}
