#pragma once
#include "IPackage.h"

#pragma once
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>




#if _WIN64 
#define CALL_CONV __stdcall
#pragma comment(lib,"slib/x64/liblua53.a")
#else
#define CALL_CONV __cdecl
#pragma comment(lib,"slib/x86/liblua53.a")
#endif

typedef int32_t(*lua_CFunction) (lua_State *L);

#include <string>
#include <map>
#include <Windows.h>

struct mPair
{
	mPair(const char* _key, lua_CFunction _value) : key(_key), value(_value) {}

	const char* key;
	lua_CFunction value;

};

typedef long(__stdcall * managed_function)(void* ptrs);
typedef mPair(__stdcall * managed_getter)(const long index);

class ManagedPackage : public IPackage
{

	std::string m_name;
	std::string m_realName;
	std::string m_path;
	std::map<std::string, lua_CFunction> m_functions;

	HMODULE m_lib;

	lua_State* m_lua;

	managed_function pckfoocount;
	managed_function pckstart;
	managed_function pckclose;
	managed_getter pckgetfoo;

public:

	ManagedPackage(lua_State* lua, std::string path, std::string name, std::string as);

	ManagedPackage(lua_State* lua, std::string path, std::string name);

	~ManagedPackage();

	void push(std::string name, cfoo foo) override;

	void initialize() override;
};