#pragma once
#include "UnmanagedPackage.h"
#include "ManagedPackage.h"
#include "luaH.h"
#include <vector>

struct Urlmon
{
	Urlmon();

	HMODULE     m_urlmon;
	std::string m_urlpath;
};

class LuaManager
{
public:
	LuaManager();
	~LuaManager();

	bool _cdecl Initialize(const int argc, char* argv[]);
	std::string _cdecl GetPath() noexcept;

	Urlmon& GetUrlmon();
	
	static LuaManager* GetInstance() noexcept;

private:

	void _cdecl OpenLibs() noexcept;

	friend int32_t _cdecl _lua_import(lua_State* state);
	friend int32_t _cdecl _lua_import_as(lua_State* state);
	friend int32_t _cdecl _lua_importmanaged(lua_State* state);
	friend int32_t _cdecl _lua_get_argument(lua_State* state);
	friend int32_t _cdecl _lua_get_argc(lua_State* state);
	friend int32_t _cdecl _lua_malloc(lua_State* state);
	friend int32_t _cdecl _lua_free(lua_State* state);
	friend int32_t _cdecl _lua_bzero(lua_State* state);
	friend int32_t _cdecl _lua_set(lua_State* state);
	friend int32_t _cdecl _lua_get(lua_State* state);
	friend int32_t _cdecl _lua_realloc(lua_State* state);
	friend int32_t _cdecl GetDebugInfo();

	lua_State* m_lua;
	Urlmon m_urlmon;

	std::string m_path;
	std::string m_file;

	int m_numberOfArguments;

	std::vector<std::string> m_args;

	std::map<std::string, IPackage*> m_pcks;
	
	
};
