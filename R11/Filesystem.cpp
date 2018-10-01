#include "pch.h"
#include "Windows.h"
#include "LuaManager.h"
#include <sys/stat.h>

namespace
{
	static lua_State* m_lua = nullptr;
}

static int32_t _cdecl Mkdir(lua_State* state)
{
	lua_pushboolean(state, CreateDirectoryA(lua_tostring(state,1), NULL));
	return 1;
}

static int32_t _cdecl __GetEnv(lua_State* state)
{
#pragma warning(disable : 4996)
	lua_pushstring(state,getenv(lua_tostring(state,1)));
	return 1;
}

static int32_t _cdecl __CopyFile(lua_State* state)
{

#pragma warning(disable : 4996)

	char buf[BUFSIZ];
	size_t size;

	std::string out = lua_tostring(state, 2);
	std::string in = lua_tostring(state, 1);

	FILE* source = fopen(in.c_str(), "rb");
	FILE* dest = fopen(out.c_str(), "wb");

	// clean and more secure
	// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

	while (size = fread(buf, 1, BUFSIZ, source)) {
		fwrite(buf, 1, size, dest);
	}

	fclose(source);
	fclose(dest);

	return 0;
}

static int32_t _cdecl __RemoveFile(lua_State* state)
{
#pragma warning(disable : 4996)
	std::string file = lua_tostring(state, 1);
	remove(file.c_str());
	return 0;
}

void _stdcall FilesystemPackageInitializer()
{

	m_lua = LuaManager::GetInstance()->m_lua;
	lua_register(m_lua, "Mkdir", Mkdir);
	lua_register(m_lua, "GetEnv", __GetEnv);
	lua_register(m_lua, "CopyFile", __CopyFile);
	lua_register(m_lua, "RemoveFile", __RemoveFile);
}