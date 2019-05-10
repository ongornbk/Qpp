#pragma once

class IPackage
{
protected:

	typedef long(__stdcall * package_start_close_count)(void* ptrs);
	typedef cpair(__stdcall * package_getfoo)(const long index);

	std::string m_name;
	std::string m_realName;
	std::string m_path;
	std::map<std::string, lua_CFunction> m_functions;

	package_start_close_count pckfoocount;
	package_start_close_count pckstart;
	package_start_close_count pckclose;
	package_getfoo            pckgetfoo;

	HMODULE m_lib;
	lua_State* m_lua;

public:
	virtual void PushFunction(std::string name, lua_CFunction foo) = 0;
	virtual void InitializePackage() = 0;
};
