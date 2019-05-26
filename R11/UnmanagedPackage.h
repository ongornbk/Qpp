#pragma once
#include "IPackage.h"


typedef long(__stdcall * package_start_close_count_function)(void* ptrs);
typedef cpair(__stdcall * package_get_foo_function)(const long index);

class UnmanagedPackage : public IPackage
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



	UnmanagedPackage(lua_State* lua, std::string path, std::string name, std::string as);

	UnmanagedPackage(lua_State* lua, std::string path, std::string name);

	~UnmanagedPackage();

	void push(std::string name, cfoo foo) override;

	void initialize() override;

};
