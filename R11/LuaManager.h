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

	friend void _stdcall ConsolePackageInitializer();
	friend void _stdcall MathPackageInitializer();
	friend void _stdcall SystemPackageInitializer();
	friend void _stdcall WindowsPackageInitializer();
	friend void _stdcall UrlmonPackageInitializer();
	friend void _stdcall FilesystemPackageInitializer();

	friend int32_t _cdecl ImportPackage(lua_State* state);

	lua_State* m_lua;

	inline void CreatePackage(std::string name, void(*initializer)(void));

	std::string m_path;

};
