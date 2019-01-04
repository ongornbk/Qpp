#pragma once
#include "PackageMap.h"
#include "luaH.h"

class LuaManager : PackageMap
{
public:
	LuaManager();
	~LuaManager();

	bool _cdecl Initialize(std::string path, std::string file) noexcept;
	bool _cdecl Execute(std::string filename) noexcept;
	std::string _cdecl GetPath() noexcept;
	
	static LuaManager* GetInstance() noexcept;

private:

	void _cdecl OpenLibs() noexcept;

	friend void CALL_CONV ConsolePackageInitializer();
	friend void CALL_CONV MathPackageInitializer();
	friend void CALL_CONV SystemPackageInitializer();
	friend void CALL_CONV WindowsPackageInitializer();
	friend void CALL_CONV UrlmonPackageInitializer();
	friend void CALL_CONV FilesystemPackageInitializer();
	friend void CALL_CONV InputPackageInitializer();
	friend void CALL_CONV XMLPackageInitializer();

	friend int32_t _cdecl ImportPackage(lua_State* state);
	friend int32_t _cdecl GetDebugInfo();

	lua_State* m_lua;

	inline void CreatePackage(std::string name, void(*initializer)(void));

	std::string m_path;

};
