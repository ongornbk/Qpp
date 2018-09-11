#pragma once
#include "PackageMap.h"
#include "luaH.h"

class LuaManager : PackageMap
{
public:
	LuaManager();
	~LuaManager();

	bool _cdecl Initialize() noexcept;
	bool _cdecl Execute(std::string filename) noexcept;
	
	static LuaManager* GetInstance() noexcept;

private:

	void _cdecl OpenLibs() noexcept;

	friend void _stdcall ConsolePackageInitializer();
	friend void _stdcall MathPackageInitializer();
	friend int32_t _cdecl ImportPackage(lua_State* state);

	lua_State* m_lua;

	inline void CreatePackage(std::string name, void(*initializer)(void));

};
